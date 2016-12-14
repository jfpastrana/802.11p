load test_1_1.mat
load test_1_2.mat
load test_1_3.mat
load test_2_1.mat
load test_2_2.mat
load test_2_3.mat

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

%             IBSS (Mbits)          OCB (Mbits)
% CCH     24   12  6  (1_1)     24   12  6  (2_1) 
% SCH4    24   12  6  (1_2)     24   12  6  (2_2)
% SCH6    24   12  6  (1_3)     24   12  6  (2_3)
%             test_1_*               test_2_*

time = [time_1_1 time_1_2 time_1_3 time_2_1 time_2_2 time_2_3];
lost_packets = [lost_packets_1_1 lost_packets_1_2 lost_packets_1_3 lost_packets_2_1 lost_packets_2_2 lost_packets_2_3];

for i=1:6
    for j=1:60
        data_rate(j,i)=(((20834*8*1514)/time(j,i))/1000000);
    end
end

for i=1:6
        mean_rate(1,i) = mean(data_rate(1:20,i));
        mean_rate(2,i) = mean(data_rate(21:40,i));
        mean_rate(3,i) = mean(data_rate(41:60,i));
        desviation(1,i) = std(data_rate(1:20,i));
        desviation(2,i) = std(data_rate(21:40,i));
        desviation(3,i) = std(data_rate(41:60,i));
end

for i=1:3
    for j=1:3
        sup=mean_rate(i,j)+desviation(i,j)*5;
        inf=mean_rate(i,j)-desviation(i,j)*5;
        range=sup-inf;
        x(j,:)=inf:range/100:sup;
        y(j,:) = normpdf(x(j,:), mean_rate(i,j), desviation(i,j));
        plot(x(j,:),y(j,:));
        hold on;
        sup=mean_rate(i,j+3)+desviation(i,j+3)*5;
        inf=mean_rate(i,j+3)-desviation(i,j+3)*5;
        range=sup-inf;
        x(j+3,:)=inf:range/100:sup;
        y(j+3,:) = normpdf(x(j+3,:), mean_rate(i,j+3), desviation(i,j+3));
        plot(x(j+3,:),y(j+3,:));
        hold on;
    end
end
title('Frequency 5900,5860 and 5920MHZ - IBSS vs OCB with 5,10 and 20MHZ bandwidth and 6,12 and 24Mbit/s');
xlabel('Bitrate (Mbit/s)');
xlabel('Probability (%)');

