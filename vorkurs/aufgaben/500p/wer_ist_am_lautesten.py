import requests
import uuid
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
import time


def get_menti_cookies(url):
    options = Options()
    options.add_argument("--headless")
    driver = webdriver.Chrome(options=options)
    driver.get(url)
    cookies = driver.get_cookies()
    cookie_dict = {c['name']: c['value'] for c in cookies}
    driver.quit()
    return cookie_dict


def send_word_cloud_response(url, value, cookie_dict):
    choice_id = str(uuid.uuid4())
    payload = {
        "response": {
            "type": "word-cloud",
            "choices": [
                {
                    "interactive_content_choice_id": choice_id,
                    "value": value
                }
            ]
        }
    }
    cookie_str = "; ".join([f"{k}={v}" for k, v in cookie_dict.items()])
    headers = {
        "accept": "application/json",
        "content-type": "application/json",
        "cookie": cookie_str,
        "origin": "https://www.menti.com",
        "referer": "https://www.menti.com/al3k17ru81pc",
        "user-agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/135.0.0.0 Safari/537.36",
        "x-identifier": cookie_dict.get("identifier1", "")
    }
    response = requests.post(url, json=payload, headers=headers)
    return response.status_code, response.text


def main(n):
    menti_url = "https://www.menti.com/al3k17ru81pc"
    api_url = "https://www.menti.com/core/audience/al3k17ru81pc/responses/v2/0198306a-e3c4-775b-9a21-4181a3cfd33d"
    for i in range(n):
        cookies = get_menti_cookies(menti_url)
        status, text = send_word_cloud_response(api_url, f"Ghostplayer_{i + 1}", cookies)
        print(f"Antwort {i + 1}: Status {status}, Antwort: {text}")
        time.sleep(1)


if __name__ == "__main__":
    main(5)
