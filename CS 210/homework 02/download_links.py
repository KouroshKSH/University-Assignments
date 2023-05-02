import os, sys, glob, re
import json
from pprint import pprint

import requests
from bs4 import BeautifulSoup as bs
import pandas as pd
import numpy as np
import uuid

from config import LINK_LIST_PATH

# Encoding for writing the URLs to the .txt file
# Do not change unless you are getting a UnicodeEncodeError
ENCODING = "utf-8"


def save_link(url, page):
    """
    Save collected link/url and page to the .txt file in LINK_LIST_PATH
    """
    id_str = uuid.uuid3(uuid.NAMESPACE_URL, url).hex
    with open(LINK_LIST_PATH, "a", encoding=ENCODING) as f:
        f.write("\t".join([id_str, url, str(page)]) + "\n")


def download_links_from_index():
    """
    This function should go to the defined "url" and download the news page links from all
    pages and save them into a .txt file.
    """

    # Checking if the link_list.txt file exists
    if not os.path.exists(LINK_LIST_PATH):
        with open(LINK_LIST_PATH, "w", encoding=ENCODING) as f:
            f.write("\t".join(["id", "url", "page"]) + "\n")
        start_page = 1
        downloaded_url_list = []

    # If some links have already been downloaded,
    # get the downloaded links and start page
    else:
        # Get the page to start from
        data = pd.read_csv(LINK_LIST_PATH, sep="\t")
        if data.shape[0] == 0:
            start_page = 1
            downloaded_url_list = []
        else:
            start_page = data["page"].astype("int").max()
            downloaded_url_list = data["url"].to_list()

    # WRITE YOUR CODE HERE
    #########################################
    # Start downloading from the page "start_page"
    # which is the page you ended at the last
    # time you ran the code (if you had an error and the code stopped)

    # For fixing the header issue (email CS210 team):
    custom_headers = {
        'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36'
    }

    # The first page is different from the rest, hence adding it manually
    first_url = "https://mfa.gov.md/en/press-release-types/comunicate-de-presa"

    response_obj = requests.get(first_url, 
                                    headers=custom_headers,
                                    allow_redirects=True)
    soup = bs(response_obj.text, 'lxml')

    for item in soup.find_all("div", {"class": "views-row"}):
        found_href = item.find("h4").find('a')["href"]
        collected_url = "https://mfa.gov.md" + found_href
        # print(collected_url)

        page = 0
        # print(page)
        
        if collected_url not in downloaded_url_list:
            print("\t", collected_url, flush=True)
            save_link(collected_url, page)
        
    # The rest are pages 2 to 28, but their URLs are 'page=1' until 'page=27', hence range(1, 28)
    base_url = "https://mfa.gov.md/en/press-release-types/comunicate-de-presa?page="

    for page_ID in range(start_page, 28):
        # print(f"\ncurrent page id is: {page_ID}")
        
        page_url = base_url + str(page_ID)
        # print(f"\npage url is: {page_url}")  

        response_obj = requests.get(page_url, 
                                    headers=custom_headers,
                                    allow_redirects=True)
        # print(f"\page text is: {response_obj.text}")
        soup = bs(response_obj.text, 'lxml')
        # print(soup)

        for item in soup.find_all("div", {"class": "views-row"}):
            # Save the collected url in the variable "collected_url"
            found_href = item.find("h4").find('a')["href"]
            collected_url = "https://mfa.gov.md" + found_href
            # print(collected_url)

            # Save the page that the url is taken from in the variable "page"
            page = page_ID
            # print(page_ID)

            # The following code block saves the collected url and page
            # Save the collected urls one by one so that if an error occurs
            # you do not have to start all over again
            if collected_url not in downloaded_url_list:
                print("\t", collected_url, flush=True)
                save_link(collected_url, page)
            
        #break
    #########################################


if __name__ == "__main__":
    download_links_from_index()
