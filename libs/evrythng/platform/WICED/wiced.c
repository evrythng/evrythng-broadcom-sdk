/*
 * (c) Copyright 2012 EVRYTHNG Ltd London / Zurich
 * www.evrythng.com
 */

#include <stdio.h>
#include <stdarg.h>
#include <wwd_crypto.h>
#include "evrythng/platform.h"

void platform_timer_init(Timer* t)
{
    t->time = 0;
}

void platform_timer_deinit(Timer* t)
{
}


char platform_timer_isexpired(Timer* t)
{
    wiced_time_t now;
    wiced_time_get_time(&now);
    return now >= t->time;
}


void platform_timer_countdown(Timer* t, unsigned int ms)
{
    wiced_time_get_time(&t->time);
    t->time += ms;
}


int platform_timer_left(Timer* t)
{
    wiced_time_t now;
    wiced_time_get_time(&now);
    int64_t res = t->time - now;
    return res < 0 ? 0 : res;
}


void platform_network_init(Network* n)
{
    n->tls_enabled = WICED_FALSE;
}


void platform_network_securedinit(Network* n, const char* ca_buf, size_t ca_size)
{
    if (!n || !ca_buf || !ca_size)
    {
        platform_printf("%s: bad args\n", __func__);
        return;
    }

    wiced_result_t rc = wiced_tls_init_root_ca_certificates(ca_buf);
    if (rc != WICED_SUCCESS)
    {
        platform_printf("%s: root CA certificate failed to initialize: %u\n", __func__, rc);
        return;
    }

    n->tls_enabled = WICED_TRUE;
}


int platform_network_connect(Network* n, char* hostname, int port)
{
    wiced_ip_address_t ip_address;
    wiced_result_t rc = -1;

    int attempt = 1;

    do
    {
        platform_printf("trying to resolve hostname (attempt %d)\n", attempt);
        if ((rc = wiced_hostname_lookup(hostname, &ip_address, 10000)) == WICED_SUCCESS)
        {
            platform_printf("hostname resolved\n");
            break;
        }
    }
    while (++attempt <= 5);

    if (rc != WICED_SUCCESS)
    {
        platform_printf("failed to resolve ip address of %s, rc = %d\n", hostname, rc);
        return rc;
    }

    if (n->tls_enabled)
        wiced_tls_init_simple_context(&n->tls_context, hostname);

    /* Create a TCP socket */
    rc = wiced_tcp_create_socket(&n->socket, WICED_STA_INTERFACE);
    if (rc != WICED_SUCCESS)
    {
        platform_printf("tcp socket creation failed, rc = %d\n", rc);
        return rc;
    }

    if (n->tls_enabled)
        wiced_tcp_enable_tls(&n->socket, &n->tls_context);

    rc = wiced_tcp_connect(&n->socket, &ip_address, port, 5000);
    if (rc != WICED_SUCCESS)
    {
        platform_printf("unable to establish connection to %s:%d, rc = %d\n", hostname, port, rc);
        goto exit;
    }

    rc = wiced_tcp_stream_init(&n->stream, &n->socket);
    if (rc != WICED_SUCCESS)
    {
        platform_printf("unable to init tcp stream, rc = %d\n", rc);
        goto exit;
    }

    rc = WICED_SUCCESS;
exit:
    if (rc != WICED_SUCCESS)
        platform_network_disconnect(n);
    return rc;
}


void platform_network_disconnect(Network* n)
{
    wiced_tcp_disconnect(&n->socket);
    wiced_tcp_stream_deinit(&n->stream);
    wiced_tcp_delete_socket(&n->socket);
}


int platform_network_read(Network* n, unsigned char* buffer, int length, int timeout)
{
    wiced_result_t rc = wiced_tcp_stream_read(&n->stream, buffer, length, timeout);
    if (rc != WICED_SUCCESS)
    {
        if (rc == WICED_TIMEOUT)
        {
            return -1;
        }
        else
        {
            platform_printf("failed to read data from tcp stream, rc = %d\n", rc);
            return 0;
        }
    }

    //platform_printf("successfully read %d bytes from tcp stream\n", length);

    return length;
}


int platform_network_write(Network* n, unsigned char* buffer, int length, int timeout)
{
    wiced_result_t rc = wiced_tcp_stream_write(&n->stream, buffer, length);
    if (rc != WICED_SUCCESS)
    {
        platform_printf("unable to write data to tcp stream, rc = %d\n", rc);
        return -1;
    }

    rc = wiced_tcp_stream_flush(&n->stream);
    if (rc != WICED_SUCCESS)
    {
        platform_printf("unable to flush tcp stream, rc = %d\n", rc);
        return -1;
    }

    //platform_printf("%s: successfully sent %d bytes to tcp stream\n", __func__, length);

    return length;
}


void platform_mutex_init(Mutex* m)
{
    if (!m)
    {
        platform_printf("%s: invalid mutex %p\n", __func__, m);
        return;
    }

    wiced_rtos_init_mutex(&m->mutex);
}


int platform_mutex_lock(Mutex* m)
{
    if (!m)
    {
        platform_printf("%s: invalid mutex %p\n", __func__, m);
        return -1;
    }

    wiced_result_t rc = wiced_rtos_lock_mutex(&m->mutex);
    if (rc != WICED_SUCCESS)
        platform_printf("%s: FAILED to lock %p: %d\n", __func__, &m->mutex, rc);
    return 0;
}


int platform_mutex_unlock(Mutex* m)
{
    if (!m)
    {
        platform_printf("%s: invalid mutex %p\n", __func__, m);
        return -1;
    }

    wiced_result_t rc = wiced_rtos_unlock_mutex(&m->mutex);
    if (rc != WICED_SUCCESS)
        platform_printf("%s: FAILED to unlock %p: %d\n", __func__, &m->mutex, rc);
    return 0;
}


void platform_mutex_deinit(Mutex* m)
{
    if (!m)
    {
        platform_printf("%s: invalid mutex %p\n", __func__, m);
        return;
    }

    wiced_rtos_deinit_mutex(&m->mutex);
}


void platform_semaphore_init(Semaphore* s)
{
    if (!s)
    {
        platform_printf("%s: invalid semaphore %p\n", __func__, s);
        return;
    }

    wiced_rtos_init_semaphore(&s->sem);
}


void platform_semaphore_deinit(Semaphore* s)
{
    if (!s)
    {
        platform_printf("%s: invalid semaphore %p\n", __func__, s);
        return;
    }

    wiced_rtos_deinit_semaphore(&s->sem);
}


int platform_semaphore_post(Semaphore* s)
{
    if (!s)
    {
        platform_printf("%s: invalid semaphore %p\n", __func__, s);
        return -1;
    }

    wiced_result_t rc = wiced_rtos_set_semaphore(&s->sem);
    if (rc != WICED_SUCCESS)
    {
        platform_printf("%s: FAILED to post semaphore %p: %d\n", __func__, &s->sem, rc);
        return -1;
    }

    return 0;
}


int platform_semaphore_wait(Semaphore* s, int timeout_ms)
{
    if (!s)
    {
        platform_printf("%s: invalid semaphore %p\n", __func__, s);
        return -1;
    }

    wiced_result_t rc = wiced_rtos_get_semaphore(&s->sem, timeout_ms);
    if (rc != WICED_SUCCESS)
    {
        //platform_printf("%s: FAILED to wait semaphore %p: %d\n", __func__, &s->sem, rc);
        return -1;
    }

    return 0;
}


static void func_wrapper(uint32_t arg)
{
    Thread* t = (Thread*)arg;
    (*t->func)(t->arg);

    platform_semaphore_post(&t->join_sem);
}


int platform_thread_create(Thread* t, 
        int priority, 
        const char* name, 
        void (*func)(void*), 
        size_t stack_size, 
        void* arg)
{
    if (!t) return -1;
    t->func = func;
    t->arg = arg;

    platform_semaphore_init(&t->join_sem);

    if (wiced_rtos_create_thread(&t->tid, priority, name, func_wrapper, stack_size, t) != WICED_SUCCESS)
        return -1;

    return 0;
}


int platform_thread_join(Thread* t, int timeout_ms)
{
    if (!t) return -1;

    if (platform_semaphore_wait(&t->join_sem, timeout_ms) != 0)
    {
        platform_printf("%s: timeout waiting for join\n", __func__);
        return -1;
    }

    return 0;
}


int platform_thread_destroy(Thread* t)
{
    if (!t) return -1;
    wiced_result_t rc = wiced_rtos_delete_thread(&t->tid);

    platform_semaphore_deinit(&t->join_sem);

    if (rc != WICED_SUCCESS) return -1;
    return 0;
}

static int stdout_mtx_inited;
static wiced_mutex_t stdout_mtx;

int platform_printf(const char* fmt, ...)
{
    if (!stdout_mtx_inited)
    {
        wiced_rtos_init_mutex(&stdout_mtx);
        stdout_mtx_inited++;
    }

    wiced_rtos_lock_mutex(&stdout_mtx);

    va_list vl;
    va_start(vl, fmt);

    char msg[512];
    unsigned n = vsnprintf(msg, sizeof msg, fmt, vl);
    if (n >= sizeof msg)
        msg[sizeof msg - 1] = '\0';

    int rc = printf("%s", msg);

    va_end(vl);

    wiced_rtos_unlock_mutex(&stdout_mtx);

    return rc;
}


void* platform_malloc(size_t bytes)
{
    return malloc(bytes);
}


void* platform_realloc(void* ptr, size_t bytes)
{
    return realloc(ptr, bytes);
}


void platform_free(void* memory)
{
    free(memory);
}


void platform_sleep(int ms)
{
    wiced_rtos_delay_milliseconds(ms);
}


int platform_rand()
{
    int random_number;
    wwd_wifi_get_random(&random_number, sizeof random_number);

    return random_number;
}
