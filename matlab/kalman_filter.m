N=50; %采样点  
   
    Xkf=zeros(1,N);%滤波处理的状态，也叫估计值
    Z=txtread('data.txt');%采集电压值
    P=zeros(1,N);%协方差矩阵


   
    %噪声
    R=0.09;

    V=sqrt(R)*randn(1,N); %测量噪声的方差分布
   %系统矩阵
   F=1;%状态转移矩阵
   G=1;%噪声驱动矩阵
   H=1;%观测矩阵
   I=eye(1); %系统为一维
   %%模拟猪的体重情况
   for k=2:N

       
       X_pre(k)=F*Xkf(k-1); %状态预测，临时值
       P_pre(k)=F*P(k-1)*F'+ G*Q*G';%协方差预测                                           
       Kg(k)=P_pre(k)*H'*inv(H*P_pre(k)*H'+ R);%计算kalman的增益  
       e=Z(k)-H*X_pre(k); %计算新息
       Xkf(k)=X_pre(k)+Kg(k)*e; %状态更新 
       P(k)=(I-Kg(k)*H)*P_pre(k)*(I-Kg(k)*H)'+Kg(k)*R*Kg(k)'; %协方差更新
  
   end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
       %%计算误差结果
       Err_messure=zeros(1,N);%用于评估温度计测量值与真值的误差
       Err_kalman=zeros(1,N);%用于评估滤波后的值与真值误差

       for k=1:N
            Err_messure(k)=abs(Z(k)-X(k));
            Err_kalman(k)=abs(Xkf(k)-X(k));
       end
       
       t=1:N;
       
       figure;
       plot(t,Z,'-ko',t,Xkf,'-g*');
       legend('采样值','KLM滤波后值');
       xlabel('采集点数');
       ylabel('采集数值');
       figure;
       plot(t,Err_messure,'-b.',t,Err_kalman,'-k*');
       legend('采集误差','估值误差');
       xlabel('采集点数');
       ylabel('数值');
       
