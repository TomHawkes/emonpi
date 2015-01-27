void RF_Setup(){
	//--------------------------------------------------Initalize RF and send out RF test packets--------------------------------------------------------------------------------------------  
  //NEED TO ADD RF MODULE AUTO DETECTION 
  delay(10);
  rf12_initialize(nodeID, RF_freq, networkGroup);                          // initialize RFM12B/rfm69CW
   for (int i=10; i>=0; i--)                                                                  //Send RF test sequence (for factory testing)
   {
     emonPi.power1=i; 
     rf12_sendNow(0, &emonPi, sizeof emonPi);
     delay(100);
   }
  rf12_sendWait(2);
  emonPi.power1=0;
 //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
}

boolean RF_Rx_Handle(){

	if (rf12_recvDone()) {						//if RF Packet is received 
	    byte n = rf12_len;
	    if (rf12_crc == 0)							//Check packet is good
	    {
	    	Serial.print(PSTR("OK"));
	    }

	    Serial.print(' ');							//Print RF packet to serial in struct format
	    Serial.print((word)rf12_hdr & 0x1F);
	    for (byte i = 0; i < n; ++i) {
	      Serial.print((word)rf12_data[i]);
	    }

	    #if RF69_COMPAT
	    // display RSSI value after packet data
	    Serial.print(PSTR(" ("));
	    Serial.print(-(RF69::rssi>>1));
	    Serial.print(PSTR(") "));
		#endif
	    	Serial.println();

	    return(1);
	}
	else
		return(0);
}

void send_RF(){

	if (cmd && rf12_canSend() ) {
	    digitalWrite(LEDpin, HIGH);
	    showString(PSTR(" -> "));
	    Serial.print((word) sendLen);
	    showString(PSTR(" b\n"));
	    byte header = cmd == 'a' ? RF12_HDR_ACK : 0;
	    if (dest)
	      header |= RF12_HDR_DST | dest;
	    rf12_sendStart(header, stack, sendLen);
	    cmd = 0;
	}

}
 
