#! /usr/bin/python3

import sys
import os
import hashlib
import urllib.parse
import bencode
import argparse

sys.argv = [sys.argv[0], '.\wget']

fileList = []
rootdir = sys.argv[1]
for root, subFolders, files in os.walk(rootdir):
    for file in files:
        fileList.append(os.path.join(root,file))
print(fileList)

result = []
new_trackers = []
trackers = []

# read from torrent files or stdin and print magnets
#a=add to file w=delete file and write in new one
out = open("magnet.txt", 'w')

for filename in fileList:
    print(filename, ":", file=sys.stderr)
    file = open(filename, "br")
    byte_stream = file.read()
    file.close()

    torrentdic = bencode.bdecode(byte_stream)
    
    # add hash info
    if "info" not in torrentdic:
        #raise ValueError("No info dict in torrent file")
        print("No info dict in torrent file")
        continue
        
    encodedInfo = bencode.bencode(torrentdic["info"])
    sha1 = hashlib.sha1(encodedInfo).hexdigest()
    result.append("xt=urn:btih:"+sha1)

    # add display name
    if "name" in torrentdic["info"]:
        quoted = urllib.parse.quote(torrentdic["info"]["name"], safe="")
        result.append("dn="+quoted)

    # add trackers list
    del trackers[:]
    if "announce-list" in torrentdic:
        for urllist in torrentdic["announce-list"]:
            trackers += urllist
    elif "announce" in torrentdic:
        trackers.append(torrentdic["announce"])
    if new_trackers:
        trackers += new_trackers

    # eliminate duplicates without sorting
    seen_urls = []
    for url in trackers:
        if [url] not in seen_urls:
            seen_urls.append([url])
            quoted = urllib.parse.quote(url, safe="")
            result.append("tr="+quoted)
    torrentdic["announce-list"] = seen_urls

    # output magnet file
    
    out.write("magnet:?"+str("&".join(result))+'\n')
    #print("magnet:?", "&".join(result))
    
    del result[:]


    
out.close()
