from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from webdriver_manager.chrome import ChromeDriverManager
import os
import time
import shutil
from urllib.parse import urlparse
from dotenv import load_dotenv
import json

# === SETTINGS ===
BASE_URL = "https://grader.nattee.net"
LOGIN_URL = BASE_URL
ROOT_DIR = os.path.dirname(__file__)

CONFIG_DIR = os.path.join(ROOT_DIR, "config.json")
with open(CONFIG_DIR, "r") as fin:
    config = json.load(fin)
PROBLEM_NUM = config["py"]["problem_num"]

PROBLEM_URL = f"{BASE_URL}/testcases/show_problem/{PROBLEM_NUM}"


DOWNLOAD_DIR = os.path.join(ROOT_DIR, "testcase")
DOWNLOAD_TEMP_DIR = os.path.join(ROOT_DIR, "temp")
os.makedirs(DOWNLOAD_DIR, exist_ok=True)
os.makedirs(DOWNLOAD_TEMP_DIR, exist_ok=True)

load_dotenv()
NAME = os.getenv("NAME")
PASSWORD = os.getenv("PASSWORD")

# === CHROME OPTIONS ===
chrome_options = Options()
chrome_options.add_argument("--headless=new")  # modern headless
chrome_options.add_argument("--window-size=1920,1080")
chrome_options.add_argument("--no-sandbox")
chrome_options.add_argument("--disable-gpu")
chrome_options.add_argument("--disable-extensions")
chrome_options.add_argument("--disable-background-networking")
chrome_options.add_argument("--blink-settings=imagesEnabled=false")
chrome_options.add_experimental_option("prefs", {
    "download.default_directory": DOWNLOAD_TEMP_DIR,
    "download.prompt_for_download": False,
    "download.directory_upgrade": True,
    "safebrowsing.enabled": True,
    "profile.default_content_setting_values.automatic_downloads": 1
})

driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()), options=chrome_options)

def wait_for_downloads(folder, timeout=60):
    # Wait until Chrome finishes all downloads in the folder.
    elapsed = 0
    while elapsed < timeout:
        if not any(fname.endswith(".crdownload") for fname in os.listdir(folder)):
            return True
        time.sleep(1)
        elapsed += 1
    return False

# === 1. LOGIN ===
print("Waiting for login page...")
driver.get(LOGIN_URL)
print("logging in...")
time.sleep(2)
driver.find_element(By.ID, "login").send_keys(NAME)
driver.find_element(By.ID, "password").send_keys(PASSWORD)
driver.find_element(By.NAME, "commit").click()
print("logged in!")
time.sleep(3)

# === 2. GO TO PROBLEM PAGE ===
print("waiting for main page...")
driver.get(PROBLEM_URL)
print("page loaded!")
time.sleep(2)

# === 3. EXTRACT ALL DOWNLOAD BUTTONS ===
print("waiting for testcase page...")
download_buttons = driver.find_elements(By.CSS_SELECTOR, "a.btn.btn-info.btn-sm")
print(f"Found {len(download_buttons)} download buttons.")

for index, btn in enumerate(download_buttons, start=1):
    href = btn.get_attribute("href")
    if not href:
        continue
    
    # Use last part of href as a temporary filename to check existence
    parsed_url = urlparse(href)
    filename = os.path.basename(parsed_url.path)
    filepath = os.path.join(DOWNLOAD_DIR, filename)

    if os.path.exists(filepath):
        print(f"Skipping existing file: {filename}")
        continue

    print(f"Downloading {filename}")
    driver.execute_script("arguments[0].click();", btn)
    time.sleep(1)  # give Chrome time to start downloading

    # Wait for Chrome to finish this download
    if wait_for_downloads(DOWNLOAD_TEMP_DIR):
        # Move from temp -> final folder if new
        temp_files = [f for f in os.listdir(DOWNLOAD_TEMP_DIR) if not f.endswith(".crdownload")]
        for f in temp_files:
            src = os.path.join(DOWNLOAD_TEMP_DIR, f)
            dest = os.path.join(DOWNLOAD_DIR, f)
            if os.path.exists(dest):
                print(f"Duplicate detected, deleting temp file: {f}")
                os.remove(src)
            else:
                shutil.move(src, dest)
                print(f"Downloaded and moved: {f}")
    else:
        print(f"Timeout waiting for {filename} to finish.")

# === 4. WAIT FOR DOWNLOADS TO FINISH ===
print("Waiting for downloads to finish...")
time.sleep(5)  # adjust if files are large

driver.quit()
print("All downloads finished and Chrome closed!")

config["cpp"]["test_case"] = f.split(".")[0]

with open(CONFIG_DIR, "w") as fout:
    json.dump(config, fout, indent=2)
    
print("Downloaded testcases recorded in config.json!")