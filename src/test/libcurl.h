#include <curl/curl.h>
#include <cstdio>
#include <cstdlib>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int download()
{
    
    CURL *curl;
    FILE *fp;
    const char *url = "http://ichart.yahoo.com/table.csv?s=600000.SS";
    const char *out_fn = "test.csv";
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(out_fn, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return 0;
}

void test_curl()
{
    download();
}
