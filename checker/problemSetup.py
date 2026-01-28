from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from webdriver_manager.chrome import ChromeDriverManager
import os
import time
import shutil
from dotenv import load_dotenv
import json
import requests
import zipfile

# === SETTINGS ===
BASE_URL = "https://grader.nattee.net"
LOGIN_URL = BASE_URL
ROOT_DIR = os.path.dirname(__file__)

CONFIG_DIR = os.path.join(ROOT_DIR, "config.json")
with open(CONFIG_DIR, "r") as fin:
    config = json.load(fin)

PROBLEM_CAT = config["py"]["problem_cat"]
PROBLEM_CAT_DIR = os.path.join(ROOT_DIR, "../" + PROBLEM_CAT)
os.makedirs(PROBLEM_CAT_DIR, exist_ok=True)

ORDER_DIR = os.path.join(ROOT_DIR, "../order.txt")
with open(ORDER_DIR, "r") as f:
    for line in f:
        if PROBLEM_CAT in line:
            PROBLEM_CODE = next(f, None).strip()
            break

DOWNLOAD_TEMP_DIR = os.path.join(ROOT_DIR, "temp")
ZIP_PATH = os.path.join(DOWNLOAD_TEMP_DIR, "problem.zip")
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

def main():
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


    # === 2. GET THE PROBLEM FILES ===
    print(f"Extracting problem: {PROBLEM_CODE}")
    problem_cells = driver.find_elements(
        By.CSS_SELECTOR,
        'tbody[data-controller="problem-name"] tr td:nth-of-type(2)'
    )

    problem_name = None
    problem_number = None
    file_link = None
    isProblem = False
    for td in problem_cells:
        problem_div = td.find_element(
            By.CSS_SELECTOR,
            "div.text-muted.font-monospace"
        )
        problem_case = problem_div.text.strip().lower().replace(" ", "")

        if problem_case == PROBLEM_CODE.lower().strip().replace(" ", ""):
            print("Problem found!")
            isProblem = True
            
            # extract testcase name
            strong = td.find_element(By.TAG_NAME, "strong")
            problem_name = strong.parent.execute_script(
                "return arguments[0].childNodes[0].textContent.trim();",
                strong
            ).strip().replace(" ", "")
            
            # extract problem statement
            pdf_link = td.find_element(
                By.CSS_SELECTOR,
                'a[href*="download/statement"]'
            ).get_attribute("href")

            # extract problem num
            problem_number = int(pdf_link.split("/")[4])

            # Try to get file link
            file_elems = td.find_elements(
                By.CSS_SELECTOR,
                'a[href*="download/attachment"]'
            )

            file_link = file_elems[0].get_attribute("href") if file_elems else None

            problem_path = f"../{PROBLEM_CAT}/{problem_name}"
            problem_dir = os.path.join(ROOT_DIR, problem_path)
            break

    if problem_name:
        print(f"Probelm name: {problem_name}")
    else:
        print("Problem not found")
        return
    
    if file_link:
        print(f"Testcase name: {PROBLEM_CODE}")
    else:
        print("Testcase not found...")

    if problem_number:
        print(f"Problem Num: {problem_number}")
    else:
        print("Problem num not found")


    config["py"]["problem_num"] = problem_number
    config["cpp"]["test_case"] = PROBLEM_CODE

    if os.path.exists(problem_dir):
        print("Problem Already Exist!! Exiting...")
        return

    # Create Problem Dir
    os.makedirs(problem_dir)

    # === 3. SETUP PROBLEM ===
    # setup .cpp
    if file_link:
        download_zip_files(file_link, problem_dir, problem_path, problem_name)
    elif isProblem:
        set_up_problem(problem_dir, problem_path, problem_name)
    else:
        print("Problem not found...")
        return
    
    # download problem statement.pdf
    download_problem_statement(pdf_link, problem_dir, problem_name)
    
    # saves config changes
    with open(CONFIG_DIR, "w") as fout:
        json.dump(config, fout, indent=2)
    print("New config for this problem have been saved!")

    print("Problem setup done!")


def download_zip_files(file_link, problem_dir, problem_path, problem_name):
    session = requests.Session()
    for cookie in driver.get_cookies():
        session.cookies.set(cookie['name'], cookie['value'])

    print("Downloading the zipfile...")
    r = session.get(file_link)
    with open(ZIP_PATH, "wb") as f:
        f.write(r.content)

    # Check if the file is a valid zip
    is_zip = zipfile.is_zipfile(ZIP_PATH)

    if is_zip:
        print("Extracting the zip file...")
        try:
            with zipfile.ZipFile(ZIP_PATH, 'r') as z:
                for file in z.namelist():
                    # Select only .cpp and .h
                    if file.endswith(".cpp") or file.endswith(".h"):
                        file_type = ".cpp" if file.endswith(".cpp") else ".h"
                        print(f"{file_type} found!")
                        extracted_path = z.extract(file, "temp_extract")

                        base_name = os.path.basename(file)
                        dest_path = os.path.join(problem_dir, base_name)
                        
                        print(f"Moving {file_type} file to {dest_path}...")
                        shutil.move(extracted_path, dest_path)

            config["cpp"]["path"] = f"{problem_path}/main.cpp"
        except Exception as e:
            print(f"Error extracting zip: {e}")

    else:
        print("File is not a zip — treating it as a .cpp file.")
        # New file path = parent/probdir.cpp   (no folder created)
        new_cpp_path = problem_dir + ".cpp"

        print(f"Renaming {ZIP_PATH} → {new_cpp_path}")
        shutil.move(ZIP_PATH, new_cpp_path)

        # Update config and STOP (do not delete the file)
        config["cpp"]["path"] = f"{problem_path}/{problem_name}.cpp"

    # CLEANUP .zip file
    if os.path.exists(ZIP_PATH):
        os.remove(ZIP_PATH)

    # CLEANUP temp_extract folder
    if os.path.exists("temp_extract"):
        shutil.rmtree("temp_extract")


def set_up_problem(problem_dir, problem_path, problem_name):
    open(f"{problem_dir}/{problem_name}.cpp", "w").close()
    config["cpp"]["path"] = f"{problem_path}/{problem_name}.cpp"


def download_problem_statement(pdf_link, problem_dir, problem_name):
    problem_path = f"{problem_dir}/{problem_name}.pdf"
    session = requests.Session()
    for cookie in driver.get_cookies():
        session.cookies.set(cookie['name'], cookie['value'])
    
    print("Downloading Problem Statement...")

    r = session.get(pdf_link, stream=True)
    r.raise_for_status()

    with open(problem_path, "wb") as f:
        for chunk in r.iter_content(chunk_size=8192):
            if chunk:
                f.write(chunk)
            
    print("Problem Statement Downloaded!")


if __name__ == "__main__":
    main()