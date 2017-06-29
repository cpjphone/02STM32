  if ((Data_Buff[0]+Data_Buff[1]+Data_Buff[2]+Data_Buff[3]) == Data_Buff[4]) 
  { 
  //加入湿度的整数和小数 
Printf(“%s”,”湿度为：”); 
  printf(“%c”, Data_Buff[0]/10+48); 
  printf(“%c”, Data_Buff[0]%10+48); 
  printf(“%c”,’.’); 
  printf(“%c”, Data_Buff[1]/10+48); 
  printf(“%c”, Data_Buff[1]%10+48); 
  printf(“\n”); 
  //加入温度的整数和小数 
Printf(“%s”,”温度为：”); 
  printf(“%c”, Data_Buff[2]/10+48); 
  printf(“%c”, Data_Buff[2]%10+48); 
  printf(“%c”,’.’); 
  printf(“%c”, Data_Buff[3]/10+48); 
  printf(“%c”, Data_Buff[3]%10+48); 
  printf(“\n”); 
     }else //校验出错 
     { 
    Printf(“%s”,check); 
     } 
   }else   
   { 
       Printf(“%s”,error); 
   } 
} 