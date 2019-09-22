import sys
import urllib.request
import urllib.parse
import json
import os
from zipfile import ZipFile

build_id = sys.argv[1]
filename = sys.argv[2]
destination = sys.argv[3]
install_folder = sys.argv[4]

url = "https://dev.azure.com/msft-ShaderConductor/public/_apis/build/builds/%s/artifacts?artifactName=%s&api-version=4.1" % (
build_id, filename)

f = urllib.request.urlopen(url)
j = json.loads(f.read().decode('utf-8'))
download_url = j["resource"]["downloadUrl"]

downloaded_file_name = os.path.join(destination, filename + ".zip")

basedir = os.path.dirname(downloaded_file_name)
if not os.path.exists(basedir):
    os.makedirs(basedir)

urllib.request.urlretrieve(download_url, downloaded_file_name)

with ZipFile(downloaded_file_name, 'r') as zipObj:
    zipObj.extractall(install_folder)
