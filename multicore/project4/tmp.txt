   for(i=0; i<max_val; i++){
      histogram_d[i] =0;
   }
   for (i=0; i<size; i++){
      histogram_d[arr_d[i]]++;
   }

   for(i=0; i<max_val; i++){
      for(j=0; j<histogram_d[i]; j++){
         arr_d[index++] = i;
      }
   }