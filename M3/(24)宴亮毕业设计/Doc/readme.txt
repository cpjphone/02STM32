  if ((Data_Buff[0]+Data_Buff[1]+Data_Buff[2]+Data_Buff[3]) == Data_Buff[4]) 
  { 
  //����ʪ�ȵ�������С�� 
Printf(��%s��,��ʪ��Ϊ����); 
  printf(��%c��, Data_Buff[0]/10+48); 
  printf(��%c��, Data_Buff[0]%10+48); 
  printf(��%c��,��.��); 
  printf(��%c��, Data_Buff[1]/10+48); 
  printf(��%c��, Data_Buff[1]%10+48); 
  printf(��\n��); 
  //�����¶ȵ�������С�� 
Printf(��%s��,���¶�Ϊ����); 
  printf(��%c��, Data_Buff[2]/10+48); 
  printf(��%c��, Data_Buff[2]%10+48); 
  printf(��%c��,��.��); 
  printf(��%c��, Data_Buff[3]/10+48); 
  printf(��%c��, Data_Buff[3]%10+48); 
  printf(��\n��); 
     }else //У����� 
     { 
    Printf(��%s��,check); 
     } 
   }else   
   { 
       Printf(��%s��,error); 
   } 
} 