load test_9_1.mat
load test_9_2.mat
load test_9_3.mat
load test_9_4.mat
load test_9_5.mat
load test_9_6.mat
load test_9_7.mat


%En los vectores "time" esta el tiempo de duracion de cada sub_test. Es
%decir sabemos que el numero de paquetes enviados fue de 20834 (menos los
%perdidos). Y como sabemos el tiempo que necesitó podemos calcular el
%bitrare. El cual es almacenado en los vectores data_rate.
%Por otro lado el vector lost_packets almacena el numero de paquetes
%perdidos por iteracion. Es decir el subtest time(1,1) perdió
%lost_packets(1,1).
%Por último, los vectores tienen longitud 60, porque se realizaron 3 test
%de 20 muestras cada uno. Por lo que han de representarse por separado,
%como test independientes, aunque esten almacenados en el mismo array

%             OCB (Mbits)      
% CCH     24   12  6  (9_1)   
% SCH4    24   12  6  (9_2) 
% SCH6    24   12  6  (9_3)
% SCH1    24   12  6  (9_4)
% SCH3    24   12  6  (9_5)
% SCH2    24   12  6  (9_6)
% SCH5    24   12  6  (9_7)
%             test_9_*     

c(1) = cellstr('CCH0');
c(2) = cellstr('SCH4');
c(3) = cellstr('SCH6');
c(4) = cellstr('SCH1');
c(5) = cellstr('SCH3');
c(6) = cellstr('SCH2');
c(7) = cellstr('SCH5');
time = [time_9_1 time_9_2 time_9_3 time_9_4 time_9_5 time_9_6 time_9_7];
lost_packets = [lost_packets_9_1 lost_packets_9_2 lost_packets_9_3 lost_packets_9_4 lost_packets_9_5 lost_packets_9_6 lost_packets_9_7];

for i=1:7
    for j=1:60
        data_rate(j,i)=(((20834*8*1500)/time(j,i))/1000000);
    end
end

for i=1:7
        mean_rate(1,i) = mean(data_rate(1:20,i));
        mean_rate(2,i) = mean(data_rate(21:40,i));
        mean_rate(3,i) = mean(data_rate(41:60,i));
        desviation(1,i) = std(data_rate(1:20,i));
        desviation(2,i) = std(data_rate(21:40,i));
        desviation(3,i) = std(data_rate(41:60,i));
        mean_lost(1,i) = mean(lost_packets(1:20,i));
        mean_lost(2,i) = mean(lost_packets(21:40,i));
        mean_lost(3,i) = mean(lost_packets(41:60,i));        
end

counter = 1;
i=1;
margin = tinv([0.025 0.975], 19);
figure
subplot(1,3,i);
for i=1:3
    for j=1:7
        sup=mean_rate(i,j)+desviation(i,j)*5;
        inf=mean_rate(i,j)-desviation(i,j)*5;
        range=sup-inf;
        x=inf:range/100:sup;
        y = normpdf(x, mean_rate(i,j), desviation(i,j));
        sprintf('\nMedia  i=%d j=%d es: %d y el intervalo de confianza:',i,j,mean_rate(i,j))
        conf = mean_rate(i,j) + margin*desviation(i,j)/sqrt(19)
        subplot(1,3,i);
        plot(x,y)
        hold on;
    end
    if i==1; texto = sprintf('Bitrate 12Mbit/s per each WLANp channel'); end
    if i==2; texto = sprintf('Bitrate 6Mbit/s per each WLANp channel'); end
    if i==3; texto = sprintf('Bitrate 3Mbit/s per each WLANp channel'); end
    title(texto);
    xlabel('Bitrate (Mbit/s)');
    ylabel('Probability density');
    legend('CCH', 'SCH4', 'SCH6', 'SCH1', 'SCH3', 'SCH2', 'SCH5');
end
figure
lost = [mean(mean_lost(:,2)) mean(mean_lost(:,5)) mean(mean_lost(:,4)) mean(mean_lost(:,6)) mean(mean_lost(:,1)) mean(mean_lost(:,7)) mean(mean_lost(:,3))];
(mean(mean_lost(:,2))*100)/20834
(mean(mean_lost(:,5))*100)/20834
(mean(mean_lost(:,4))*100)/20834
(mean(mean_lost(:,6))*100)/20834
(mean(mean_lost(:,1))*100)/20834
(mean(mean_lost(:,7))*100)/20834
(mean(mean_lost(:,3))*100)/20834

bar(lost,1,'stacked','w')
title('Mean of lost packets per ITS channel');
xlabel('Channels');
ylabel('Number of lost packets');