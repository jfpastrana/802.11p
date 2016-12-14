clear all

load test_4.1.txt
load test_4.2.txt
load test_5.1.txt
load test_5.2.txt

save('test_4_1.mat','test_4_1')
save('test_4_2.mat','test_4_2')
save('test_5_1.mat','test_5_1')
save('test_5_2.mat','test_5_2')

load results_test1.1.txt
load results_test1.2.txt
load results_test1.3.txt
load results_test2.1.txt
load results_test2.2.txt
load results_test2.3.txt

load results_test9.1.txt
load results_test9.2.txt
load results_test9.3.txt
load results_test9.4.txt
load results_test9.5.txt
load results_test9.6.txt
load results_test9.7.txt

time_1_1=results_test1_1(:,1);
lost_packets_1_1 = results_test1_1(:,2);
save('test_1_1.mat','time_1_1','lost_packets_1_1');

time_1_2=results_test1_2(:,1);
lost_packets_1_2 = results_test1_2(:,2);
save('test_1_2.mat','time_1_2','lost_packets_1_2');

time_1_3=results_test1_3(:,1);
lost_packets_1_3 = results_test1_3(:,2);
save('test_1_3.mat','time_1_3','lost_packets_1_3');

time_2_1=results_test2_1(:,1);
lost_packets_2_1 = results_test2_1(:,2);
save('test_2_1.mat','time_2_1','lost_packets_2_1');

time_2_2=results_test2_2(:,1);
lost_packets_2_2 = results_test2_2(:,2);
save('test_2_2.mat','time_2_2','lost_packets_2_2');

time_2_3=results_test2_3(:,1);
lost_packets_2_3 = results_test2_3(:,2);
save('test_2_3.mat','time_2_3','lost_packets_2_3');

time_9_1=results_test9_1(:,1);
lost_packets_9_1 = results_test9_1(:,2);
save('test_9_1.mat','time_9_1','lost_packets_9_1');

time_9_2=results_test9_2(:,1);
lost_packets_9_2 = results_test9_2(:,2);
save('test_9_2.mat','time_9_2','lost_packets_9_2');

time_9_3=results_test9_3(:,1);
lost_packets_9_3 = results_test9_3(:,2);
save('test_9_3.mat','time_9_3','lost_packets_9_3');

time_9_4=results_test9_4(:,1);
lost_packets_9_4 = results_test9_4(:,2);
save('test_9_4.mat','time_9_4','lost_packets_9_4');

time_9_5=results_test9_5(:,1);
lost_packets_9_5 = results_test9_5(:,2);
save('test_9_5.mat','time_9_5','lost_packets_9_5');

time_9_6=results_test9_6(:,1);
lost_packets_9_6 = results_test9_6(:,2);
save('test_9_6.mat','time_9_6','lost_packets_9_6');
 
time_9_7=results_test9_7(:,1);
lost_packets_9_7 = results_test9_7(:,2);
save('test_9_7.mat','time_9_7','lost_packets_9_7');