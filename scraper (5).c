#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#define MAX_PRODUCTS 16
#define NUM_URLS 8

char * urls[NUM_URLS] = {
    "https://scrapeme.live/shop/",
    "https://scrapeme.live/shop/page/2/",
    "https://scrapeme.live/shop/page/3/",
    "https://scrapeme.live/shop/page/4/",
    "https://scrapeme.live/shop/page/5/",
    "https://scrapeme.live/shop/page/6/",
    "https://scrapeme.live/shop/page/7/",
    "https://scrapeme.live/shop/page/8/"
};

char * folder_names[NUM_URLS] = {};
struct CURLResponse {
  char * html;
  size_t size;
};


static size_t WriteHTMLCallback(void * contents, size_t size, size_t nmemb, void * userp) {
  size_t realsize = size * nmemb;
  struct CURLResponse * mem = (struct CURLResponse * ) userp;
  char * ptr = realloc(mem -> html, mem -> size + realsize + 1);
   if (!ptr) {
    printf("Not enough memory available (realloc returned NULL)\n");
    return 0;
  }
  mem -> html = ptr;
  memcpy( & (mem -> html[mem -> size]), contents, realsize);
  mem -> size += realsize;
  mem -> html[mem -> size] = 0;

  return realsize;
}

struct CURLResponse GetRequest(CURL * curl_handle,
  const char * url) {
  CURLcode res;
  struct CURLResponse response;

  // initialize the response
  response.html = malloc(1);
  response.size = 0;

  // specify URL to GET
  curl_easy_setopt(curl_handle, CURLOPT_URL, url);

  // send all data returned by the server to WriteHTMLCallback
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteHTMLCallback);

  // pass "response" to the callback function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void * ) & response);

  // set a User-Agent header
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36");

  // perform the GET request
  res = curl_easy_perform(curl_handle);

  // check for HTTP errors
  if (res != CURLE_OK) {
    fprintf(stderr, "GET request failed: %s\n", curl_easy_strerror(res));
  }

  return response;
}

size_t write_data(void * ptr, size_t size, size_t nmemb, FILE * stream) {
  return fwrite(ptr, size, nmemb, stream);
}

void download_image(char * URL, char * OUTPUT_FILE) {
  CURL * curl;
  FILE * fp;
  CURLcode res;

  // Initialize curl session
  curl = curl_easy_init();

  if (!curl) {
    fprintf(stderr, "Failed to initialize curl\n");
  }

  // Open file for writing
  fp = fopen(OUTPUT_FILE, "wb");

  if (!fp) {
    fprintf(stderr, "Failed to open file for writing\n");
    curl_easy_cleanup(curl);
  }

  // Set URL to fetch
  curl_easy_setopt(curl, CURLOPT_URL, URL);

  // Set write function
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

  // Set file to write to
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

  // Perform the request
  res = curl_easy_perform(curl);

  // Check for errors
  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  } else {
    printf("Image successfully downloaded and saved as %s\n", OUTPUT_FILE);
  }

  // Clean up
  curl_easy_cleanup(curl);

  fclose(fp);
}

void scrap_images(char * url, char * folder_name) {
  char cmd[32];
  sprintf(cmd, "mkdir -p %s", folder_name);
  system(cmd);

  // initialize curl globally
  curl_global_init(CURL_GLOBAL_ALL);

  // initialize a CURL instance
  CURL * curl_handle = curl_easy_init();

  // get the HTML document associated with the page
  struct CURLResponse response = GetRequest(curl_handle, url);

  // parse the HTML document returned by the server
  htmlDocPtr doc = htmlReadMemory(response.html, (unsigned long) response.size, NULL, NULL, HTML_PARSE_NOERROR);
  xmlXPathContextPtr context = xmlXPathNewContext(doc);

  // get the product HTML elements on the page
  xmlXPathObjectPtr productHTMLElements = xmlXPathEvalExpression((xmlChar * )
    "//li[contains(@class, 'product')]", context);

  // iterate over them and scrape data from each of them
  for (int i = 0; i < productHTMLElements -> nodesetval -> nodeNr; ++i) {
    // get the current element of the loop
    xmlNodePtr productHTMLElement = productHTMLElements -> nodesetval -> nodeTab[i];

    // set the context to restrict XPath selectors to the children of the current element
    xmlXPathSetContextNode(productHTMLElement, context);

    xmlNodePtr imageHTMLElement = xmlXPathEvalExpression((xmlChar * )
      ".//a/img", context) -> nodesetval -> nodeTab[0];

    char * image_url = (char * )(xmlGetProp(imageHTMLElement, (xmlChar * )
      "src"));

    char filename[256];
    sprintf(filename, "%s/Image_%d.jpg", folder_name, i + 1);

    download_image(image_url, filename);

    free(image_url);
  }

  // free up the allocated resources
  free(response.html);
  xmlXPathFreeContext(context);
  xmlFreeDoc(doc);
  xmlCleanupParser();

  // cleanup the curl instance
  curl_easy_cleanup(curl_handle);

  // cleanup the curl resources
  curl_global_cleanup();
}

int main(void) {
  for (int i = 0; i < NUM_URLS; ++i) {
    char folder_name[32];
    sprintf(folder_name, "IMAGES_%d",i+1);
    scrap_images(urls[i], folder_name);
  }
  return 0;
}
