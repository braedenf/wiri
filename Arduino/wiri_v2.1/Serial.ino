
//============

/*
 * When Serial is available start reading the data,
 * When data is received and in progress loop over each byte
 * making sure the first byte equals the '<' maker and the last byte
 * is the '>' marker. Once the end marker is reached then terminate the string and
 * stop receiving serial and mark that new data is available to parse the gathered data.
 */

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

/*
 * When new data is received start parsing the data by storing
 * characters into a string delinated by commas.
 * The first value is the toggle
 * The second value is the mouse X postion
 * The third value is the mouse Y position
 * Finally the forth value is the WAVEFORM type.
 */

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    //strcpy(toggle, strtokIndx); // copy it to toggle
    toggle = atoi(strtokIndx);
    
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    mouseX = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    mouseY = atof(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    waveType = atof(strtokIndx);

}

//============

/*
 * For debugging purposes, show the parsed data in the Serial monitor.
 */

void showParsedData() {
    Serial.print("( ");
    Serial.print("Toggle: ");
    Serial.print(toggle);
    Serial.print(", ");
    Serial.print("MouseX: ");
    Serial.print(mouseX);
    Serial.print(", ");
    Serial.print("MouseY: ");
    Serial.print(mouseY);
    Serial.print(", ");
    Serial.print("WaveType: ");
    Serial.print(waveType);
    Serial.println(" )");
}
