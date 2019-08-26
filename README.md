KickassCopy
===========

KickassCopy is a collection of tools (often written in C++) to dump torrent pages into an SQL database.
I started this project 2 days after the December 2014 Raid against PirateBaybefore I even heard of the openbay project.

## Fautures
* Getting live Database updates by using RSS feeds from famous Torrent sites.
* Creating Download lists for HTTrack
* Brut force search downloading (uses much CPU on the Sphinx Server if you are not careful with this you could DOS the site you are trying to download or will be banned)
* Reading the maximal amount of Search results from the first page of every bruteforce result and make a list with every existing site for HTTrack.
* Convert the downloaded HTML files into an importable, with MySQL compatible, CSV file or in a magnet/torcache URL-list
* Duplicate filter to remove equal torrent search result (ca. 50% if you use brute force)
* Converter from dalydumps (Torrent api) into importable CSV files.
* A lot of useful tools to convert, split and edit raw data, csv files and HTTrack download list. I recommend not downloading more than 999999 in one step because if you do you have to remove the HTTrack max links limit in the settings and also the ban possibility is higher if you download that much in one step. Try to always to get the best download speed by using more connections and a high download rate but try not to slowdown the server.
* Notes with useful MySQL commands and import instructions.

## What you need to build:
- A windows PC – Other operating systems will need slight code modifications
- A C++ Compiler like MinGW
- An IDE - I'm using Code::Blocks

## How to build:
Just open the main .cpp file of the subproject in your favorite IDE. I don't use any headers or libraries to make this step as easy as possible. If you use Code::Blocks as your IDE you can just open the Code::Blocks project and it should be ready to build.
