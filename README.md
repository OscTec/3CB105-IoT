%IoT
%For these devices to function the follwing code must be used in     %MATTLAB Analysis
%Replace ??? with the correct Ids and API Keys
%This is ThingSpeak analysis code

readChannelID = ???; %put height channel ID here
readAPIKey = '???'; %put height channel read API key here
writeChannelID = [???]; %put shower channel ID here
writeAPIKey = '???'; %put shower channel write API key here

data = thingSpeakRead(readChannelID, 'ReadKey', readAPIKey);

if date >= 150 %Shower is empty
    analyzedData = 20; %This will set the servo to 20 degrees 
elseif date > 63 & date < 65 %if valus eis between 63 and 65 set to 90 degrees
    analyzedData = 90;
elseif date > 60 & date < 63
    analyzedData = 80;
elseif date > 50 & date < 60
    analyzedData = 70;      
else
    analyzedData = 75; %if nothering is true set degrees to 75
end

if data < 150 %if height is less than 150cm
    occupied = true %set occupied to true
else %else set it to false
    occupied = false   
end
%put data and occupied into another channel for the devices to read from
thingSpeakWrite(writeChannelID,[occupied,analyzedData],'Fields',[1,2],'Writekey',writeAPIKey);