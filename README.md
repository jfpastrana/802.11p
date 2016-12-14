# README #
* **Organisation**: Escuela Técnica Superior de Ingenieros de Telecomunicaciones - University of Valladolid

### What is this repository for? ###

* This repository has been the result of the Master Thesis: "802.11p standard and V2X applications on commercial Wi-Fi cards". In this repository it can be find all the documentacion and sources to implement the 802.11p standard. As well as, it has been integrated part of the upper protocolos. Explaining how can be decode the received packets using Wireshark. Besides, it has been tested that it is possible use commercial Wi-Fi wireless cards for 802.11p standard.
* Version 1.0

### How do I get set up? ###
* The implementation of this project is only possible using specific Wi-Fi wireless cards, which physically they support the 802.11p as well as their software driver is Open Source. These wireless cards are described in the document "/802.11p/Documentation/Wireless_cards.pdf".
* **Summary of set up**: The main document of the proyect can be found in "Documentation" folder. This document is the final report wrote for this proyect.
* **Configuration**: A short tutorial is explained in the file "/802.11p/Documentation/802.11p_guide.pdf". It explains how deploy the 802.11p standard in your Linux device. How to take the first steeps to install all the programs. 
* **Dependencies**: The software dependencies are described in the file "/802.11p/Documentation/802.11p_guide.pdf".  
* **How to run tests**: This part is explained in the chapter X of "802.11p standard and V2X applications on commercial Wi-Fi cards" thesis.
* **Deployment instructions**: This part is explained in the chapter X of "802.11p standard and V2X applications on commercial Wi-Fi cards" thesis.
* **Repository structure**: It is explained below how is divided the repository and where it can be found every interesting file.
  * Documentation
    * 802.11p_guide.pdf: A short view of how implement 802.11p standard in any Linux distribution.
    * 802.11p_standard_and_V2X_applications.pdf: Final Master Thesis Report. 
  * Real_ITS: ".pcap" files wich they store data from a real ITS station. Mainly, GeoNetwork, IVI, DENM and CAM protocols.
  * Software: The programs that have been developed during the project.
    * Channel_switching:
      * Receiver_ITS
        * simulation.c: This program was developed for a receiver station with two wireless cards. One of the wireless cards it is always listening in CCH and the other is commuting between channels. It is read the field number 14 of the received frame. And it will be hop to channel specified on this field.
      * Sender_ITS
        * RSU.c: This program simulate the switching channel of a Road Side Unit, it only need one wireless card and the sequence of how is selected the next channel to send a packet is described in the next Figure.
![StateMachine.png](https://bitbucket.org/repo/zbyqKG/images/2270663200-StateMachine.png)
      * get_rssi:
        * rssi.sh: It is only a bash script which dissects the output of the command IW in order to take the RSSI parameter. Because, it did not find a directly method to get the power using the Netlink socket.
      * pcap_examples: Examples which use the pcap library.
        * dump_packets.c: Example of a program to store the data received from one of the interfaces into a defined .pcap file.  
      * scripts: In this folder it can be found the programs that were lunch in the ITS devices during the proyect. Mostly, it was used to test and developed anther kind of projects that they cannot be installed into the BP2 for problems with the dependencies.
        * bin
          * shoutdown_usb.sh: Script to kill the service “usb”.
          * shutdown_wlanp.sh: Script to kill the service “wlanp”.
          * start_wlanp.sh: Script to lunch the service “wlanp”.
          * start_usb.sh: Script to lunch the service “usb”
        * telegram
          * toSend: Temporal folder used for action.lua
          * read_packets.c: Program lunched by action.lua, this program is able to read a packet from the ocb0 interface, dissect a GeoNetowrking packet to extract the important information and save this information into a temporal file stored in the folder “toSend”. 
        * usb: Service to store the .pcap captures into a specific USB dongle. 
        * wlanp: Service to create a virtual OCB interface from WLAN0.
      * tg: Folder of the Telegram client.
        * scripts: 
          * action.lua: Program that when receives a message “ping” from one of the defined sources (id1, id2, id3) it lunches the program “read_packet” and send the information to the same source who asks for it.
       * Send_data: Programs those are able to send data using the ocb0 interface.
          * sendRawEth.c: Send data using an Ethernet socket. It can be configurable the source and destination MAC as well as the data field.
          * sendRawGeo.c: It is sent an example of GeoNetworking message.
### Contribution guidelines ###

* Writing tests
* Code review
* Other guidelines

### Who do I talk to? ###

* **Author**: Javier Fernández Pastrana 
* **Email**: javier.fernandez.pastrana@alumnos.uva.es
