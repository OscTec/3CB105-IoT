IoT
For these devices to function the follwing code must be used in MATLAB Analysis
Replace ??? with the correct Ids and API Keys


readChannelID = ???;
readAPIKey = '???';
writeChannelID = [???];
writeAPIKey = '???';

data = thingSpeakRead(readChannelID, 'ReadKey', readAPIKey);

if date >= 180
            analyzedData = 20;
elseif date > 100 & date < 180
            analyzedData = 100;
        else
            analyzedData = 150;
end

if data < 220
    occupied = true
else 
    occupied = false   
end

thingSpeakWrite(writeChannelID,[occupied,analyzedData],'Fields',[1,2],'Writekey',writeAPIKey);
