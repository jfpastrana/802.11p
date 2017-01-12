clear all
load rssi.mat;

difference=diff(seconds);
jump=find(difference>115);
tmp = [1 jump'];
indices = tmp';
power = power*(-1);

n_test = size(indices); 
for i=1:n_test(1,1)-1
    mean_power(i) = mean(power(indices(i,1):indices(i+1,1)-1));
    st_dev(i) = std(power(indices(i,1):indices(i+1,1)-1));
    margin=tinv([0.025 0.975],indices(i+1,1)-indices(i,1)-1);
    inferior_margin(i)=mean_power(i)+margin(1)*st_dev(i)/sqrt(indices(i+1,1)-indices(i,1));
    superior_margin(i)=mean_power(i)+margin(2)*st_dev(i)/sqrt(indices(i+1,1)-indices(i,1));   
end

figure
subplot(1,2,1);
plot (3:17,mean_power,'o')
interval=[inferior_margin; superior_margin]';
hold on
for i=1:length(mean_power)
    interval(i,:)
    plot([i+2,i+2],interval(i,:),'-+b');
end

% Here we begin the linear regression
x=3:17;
X=[ones(length(x),1) x'];
linear_regression=X\mean_power';
slope=linear_regression(2);
interception=linear_regression(1);
plot(x,slope*x+interception,'r')
title('Maximum likelihood of power received per power transmission selected and his Linear regression');
xlabel('Power selected (dBm)from 3dBm until 17dBm');
ylabel('Power received (dBm)');

%Calculamos ahora las gráficas de cada gaussina
%figure
%hold on
subplot(1,2,2);
for i=1:n_test(1,1)-1
    sup=mean_power(i)+st_dev(i)*5;
    inf=mean_power(i)-st_dev(i)*5;
    range=sup-inf;
    x=inf:range/100:sup;
    y=normpdf(x,mean_power(i),st_dev(i));
    hold on
    plot (x,y)
end
legend('3dBm','4 dBm','5 dBm', '6 dBm', '7 dBm', '8 dBm', '9 dBm', '10 dBm', '11 dBm', '12 dBm', '13 dBm', '14 dBm', '15 dBm', '16 dBm', '17 dBm') 
title('Probability density function per power transmission selected');
xlabel('Power received (dBm) from 3dBm until 17dBm');
ylabel('Probability density');
