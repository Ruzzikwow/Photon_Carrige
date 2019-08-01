#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



char *blFtostr(char  *str,
                int   maxlen,
                double  x,
                int   precision)
 {
    char   fmt[8],
           *ptr;
    int    i;
    double   val;
    
    /* If usiing e-form, check whether string will fit                   */
    if(precision > 0)
    {
       double lgx;
       int  len;
       
       lgx = log10(x);       /* Number of pre-dp places - 1         */
       if(lgx < 0.0) lgx = 0.1;   /* i.e. If x<1.0, 1 p-dp after rounding*/
       len = (int)ceil(lgx);      /* Round up to number of pre-dp places */
       len += precision + 2;      /* dp and '\0'                         */
       
       if(len > maxlen)           /* String too long                     */
       {
          if(precision == 0)   precision  = -1;  /* Set to -1            */
          else                 precision *= -1;  /* Switch to e-form     */
       }
		 }
    /* 09.10.97 Added check on 0 precision                               */
    if((precision != 0) && (precision < -1*(maxlen-5))) 
       precision = -1*(maxlen-5);
 
    if(precision > 0)             /* 09.10.97 Changed from >=            */
    {
       sprintf(fmt,"%%.%dlf",precision);
    }
    else
    {
       precision = abs(precision);
       sprintf(fmt,"%%.%dle",precision);
    }
    if(precision == 0)            /* Added 09.10.97                      */
    {
       sprintf(str,"%d",(int)(x+0.5));
    }
    else
    {
       sprintf(str,fmt,x);
    }
    
 
    if(!strchr(str,'e'))                   /* Not e-form                 */
    {
       if(precision == 0)
       {
          /* Terminate string at .                                       */
          ptr = str;
          while(*ptr)
          {
             if(*ptr++ == '.')
             {
                *ptr = '\0';
                break;
             }
          }
       }
       else
       {
          if((ptr = strchr(str,'.'))!=NULL)   /* Contains a decimal point*/
          {
             ptr++;
             for(i=0; *ptr; ptr++, i++);
             if(i<precision)               /* Fewer places than precision*/
             {
                /* Pad with zeros                                        */
                for( ; i<precision; i++) *ptr++ = '0';
                *ptr = '\0';
             }
          }
          else                             /* No decimal point           */
          {
             /* Move to end of string                                    */
             for(ptr = str; *ptr; ptr++);
             /* Put in a dp                                              */
             *ptr++ = '.';
             /* Pad with zeros                                           */
             for(i=0; i<precision; i++) *ptr++ = '0';
             *ptr = '\0';
          }
       }
    }
       
    if(str[strlen(str)-1] == '.') str[strlen(str)-1] = '\0';
    
    /* Strip any leading spaces                                          */
    /* 03.06.05 Tidied loop for gcc 3.2.2                                */
    /* 30.09.05 Fixed the warning: operation on `i' may be undefined     */
    while(str[0] == ' ')
    {
       i=0;
       while(i<strlen(str))
       {
          str[i] = str[i+1];
          i++;
       }
    }
    
 
    /* Remove any leading minus sign if value is 0.0                     */
    sscanf(str,"%lf",&val);
    if(val == 0.0 && str[0] == '-')
    {
       /* 03.06.05 Tidied loop for gcc 3.2.2                             */
       /* 30.09.05 Fixed the warning: operation on `i' may be undefined  */
       i=0;
       while(i<strlen(str))
       {
          str[i] = str[i+1];
          i++;
       }
    }
 
    return(str);
 }
