load switch_time.mat
load timeVSpercent.mat

len = size(packet_time_stamp)

for i=1:len(1,1)
    if packet_time_stamp(i,1) > channel_time_stamp(i,1)
        elapsed_time(i,1) = (1+channel_time_stamp(i,1))-packet_time_stamp(i,1);
    else
        elapsed_time(i,1) = channel_time_stamp(i,1) - packet_time_stamp(i,1);
    end
end

mean_time = mean(elapsed_time);
des_time=std(elapsed_time);
sup=mean_time+des_time*5;
inf=mean_time-des_time*5;
range=sup-inf;
x=inf:range/100:sup;
y=normpdf(x,mean_time,des_time);
figure
subplot(1,2,1);
%texto= sprintf('The mean elapsed time to change to other is: %.2f',mean_time);
%legend('The mean elapsed time to change to other is: %.2f',mean_time);
plot(x,y);
text(mean_time,max(y),' \leftarrow 0.0322 seconds')
xlabel('Elapsed time to switch the channel (seconds)');
ylabel('Probability density');
title('Probability density function of switching channel time');

%figure
subplot(1,2,2);
plot(time_between_packets,packets_percent);
hold on
plot(time_between_packets,packets_percent,'o');
time_between_packets
packets_percent
text(time_between_packets(5),packets_percent(5),'  100% packets received - 40ms \rightarrow ')
text(time_between_packets(6),packets_percent(6),'  70% packets receied - 32ms \rightarrow ')
xlabel('Elapsed time between sent packets (seconds)');
ylabel('% of computed packets');
title('Cumputed packets VS elapsed time');

