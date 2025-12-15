s = serialport("COM5",115200);
while true
    babyHR = 130 + randi(20);
    writeline(s, num2str(babyHR));
    pause(1);
end
