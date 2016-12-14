load test_1_1.mat


for i=1:60
    data_rate_1_1(i,1)=(((20834-lost_packets_1_1(i,1))*8*1500)/time_1_1(i,1))/1000000;
end
data_rate_1_1 = sort(data_rate_1_1);

for i=0:20:40
    [valores_unicos, ind_unicos] = unique(data_rate_1_1(1+i:20+i));
    figure;
    bar(valores_unicos, ind_unicos,0.1,'k')
    set(gca,'XtickLabel',valores_unicos);
end


    
