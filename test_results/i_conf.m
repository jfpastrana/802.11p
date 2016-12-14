
time = power;

for j=1:20
    data_rate(j,1)=(((20834*8*1514)/time(j,1))/1000000);
end
sample_size = size(data_rate);
mean_rate = mean(data_rate(1:20,1));
st_dev = std(data_rate(1:20,1));
sup=mean_rate+st_dev*5;
inf=mean_rate-st_dev*5;
range=sup-inf;
x=inf:range/100:sup;
y=normpdf(x,mean_rate,st_dev);


margin = tinv([0.025 0.975], sample_size(1,1)-1);
conf = mean_rate + margin*st_dev/sqrt(sample_size(1,1))
plot(x,y)