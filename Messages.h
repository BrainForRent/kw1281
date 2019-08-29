//No original datasheet for this, just blind copy-paste from VAG-COM 3B1-907-044.lbl
//; VAG-COM Measuring Block Labels file.
//; Golf IV Climatronic (A/C) 3B1 907 044B
//;
//; contributed by Jens Ulmer, 27.08.2001
//;

	 CString  Group[8]={
                         "General",
                         "Temperature Regulator Flap Motor (V68)", 
                         "Central Air Flap Motor (V70)", 
                         "Footwell/Defroster Flap Motor (V85)", 
                         "Air Flow Flap Motor (V71)", 
                         "Temperatures I && Photo Sensor", 
                         "Temperatures II", 
                         "Voltages"};
	 
	 CString  Parameters[32]={
                         "Compressor",     
                         "Engine Speed,recognized", 
                         "Vehicle Speed", 
                         "Standing Time",
                         		 
                         "Measured Value", 
                         "Specified Value",         
                         "Flap Position (air supply cooled)", 
                         "Flap Position (air supply heated)",
                         		 
                         "Measured Value", 
                         "Specified Value",         
                         "Flap Position (air flow to panel)", 
                         "Flap Position (air flow to footwell)",
                         		 
                         "Measured Value", 
                         "Specified Value",         
                         "Flap Position (air flow to footwell)", 
                         "Flap Position (air flow to defrost.)",
                         		 
                         "Measured Value", 
                         "Specified Value",         
                         "Flap Position (fresh air flow)", 
                         "Flap Position (recirculating)",
		 
                         "Calculated Temperature (G89)~(G17)",	 
                         "Fresh Air Duct,Temperature (G89)", 
                         "Outside Air,Temperature (G17)",	
                         "Sunlight Photo,Sensor (G107)",
                         		 
                         "Center Vent (G191)",	 
                         "Footwell (G192)",		 
                         "Dash Panel (G56)", 
                         "Evaporator (G263)",
                         		 
                         "Fresh Air Blower,(V2 - specified)",
                         "Fresh Air Blower,(V2 - actual)",
                         "Voltage,(Terminal 15)",
                         "Voltage,A/C Clutch (N25)"};
                         	                    	 
     CString  Units[32]={
                         "",
                         "",
                         "km/h (0...255)",
                         "",
                         
                         "(0...255)",
                         "(0...255)",
                         "(150...250)",
                         "(5...100)",
                         
                         "(0...255)",
                         "(0...255)",
                         "(150...250)",
                         "(5...100)",
                         
                         "(0...255)",
                         "(0...255)",
                         "(150...250)",
                         "(5...100)",
                         
                         "(0...255)",
                         "(0...255)",
                         "(150...250)",
                         "(5...100)",

                         "C",
                         "C",
                         "C",
                         "% (0...120)",
                         
                         "C",
                         "C",
                         "C",
                         "C",
                         							
                         "",
                         "V, ± 0.8 V to Specification (in Field 1) ",
                         "V",
                         "V"};
                         	                    	 
     CString  ColumnName[5] = {
                         "G/r", 
                         "Group name", 
                         "Parameter name", 
                         "Value", 
                         "Unit"};
                         
     CString  EngineSpeed[2]={
                         "NO",
                         "YES"};
                                
     CString  FanSpeed[8]={
                         "OFF", 
                         "Speed 7", 
                         "Speed 6", 
                         "Speed 5", 
                         "Speed 4", 
                         "Speed 3", 
                         "Speed 2", 
                         "Speed 1"};
    //
    //No original datasheet for this, just blind copy-paste from "Eigendiagnose an der VW Climatronic von Hella"
    //Original Author: Roger; Location: Greater Frankfurt am Main Posted on: 09-09-2004 
    //Compressor-Cutout-Codes (Appendix to Channel 19)
    
    CString CompressorStatus[19]={
                 /*0*/  "Running",
                 /*1*/  "OFF by Pressure Switch (F129): Refrigerant Pressure too High",
                 /*2*/  "OFF by Fresh Air Blower (V2) or Fresh Air Blower Control Module (J126) Malfunction",
                 /*3*/  "OFF by Pressure Switch (F129): Refrigerant Pressure too Low",
                 /*4*/  "OFF by Missing Engine Speed Signal",
                 /*5*/  "OFF 4s after Motor Start",
                 /*6*/  "OFF by ECON Button",
                 /*7*/  "OFF by OFF Button (via Fresh Air Blower Speed Buttons)",
                 /*8*/  "OFF by Ambient Temperature below 3 °C (37 °F)",
				 /*9*/  "?",
                 /*10*/ "OFF by Voltage below 9.5 V",
                 /*11*/ "OFF by Coolant Temperature too High > 118°C",
                 /*12*/ "OFF by Engine OR Transmission Control Module (Motronic Interface)",
                 /*13*/ "OFF by Terminal 15 Voltage > 17 V",
                 /*14*/ "OFF by Evaporator Temperature",
                 /*15*/ "OFF by Control Module Coding incorrect",
                 /*16*/ "OFF by Compressor current",
                 /*17*/ "OFF by Pressure Sensor Malfunction",
                 /*18*/ "?OFF by Climate Control Module (J255) Malfunction"};

