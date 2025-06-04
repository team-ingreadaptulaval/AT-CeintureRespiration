

//  instanc of the haptic module
     





// void vib_feedback(void)
// {
//     if ((currentMillisStart - vib_timing) < vib_delay1 && vib_flag == 1)
//     {
//       digitalWrite(motorPin,HIGH);   
//     }
//     else if ((currentMillisStart - vib_timing) < (vib_delay1+vib_delay2) && vib_flag == 1)
//     {
//       digitalWrite(motorPin,LOW); 
//     }
//     else if ((currentMillisStart - vib_timing) < (vib_delay1+vib_delay2+vib_delay1) && vib_flag == 1)
//     {
//       digitalWrite(motorPin,HIGH); 
//     }
//     else if ((currentMillisStart - vib_timing) < (vib_delay1+vib_delay2+vib_delay1+vib_delay2) && vib_flag == 1)
//     {
//       digitalWrite(motorPin,LOW); 
//     }
//     else if ((currentMillisStart - vib_timing) < (vib_delay1+vib_delay2+vib_delay1+vib_delay2+vib_delay3) && vib_flag == 1)
//     {
//       digitalWrite(motorPin,LOW); 
//     }
//     else if (vib_flag==1)
//     {
//       vib_timing = currentMillisStart;
//       vib_counter ++;
//     }
//     else if (vib_flag == 0)
//     {
//       digitalWrite(motorPin,LOW); 
//     }
  
//     if (vib_counter>=4)
//     {
//       vib_flag = 0;
//       vib_counter = 0;
//     }
//   }