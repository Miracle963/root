#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <jni.h>

#include <sys/socket.h>

int checkIsVulnerable()
{
    int sFd = socket(2, 1, 6);
    if(sFd < 0)
    {
        return errno == 0 || errno == 1 ? -1 : errno;
    }

    int ret = -1;

    //Random kernel address
    uint32_t address = 0xc02278cc;

    int e1;
    errno = 0;

    ret = setsockopt(sFd, 0, SO_REUSEADDR, (void*)address , 1);
    e1 = errno;

    printf("ret : %d",ret);
    printf("errno: %d\n", errno);
    perror("setsockopt");

    if(ret == -1 && e1 == 14) return 0; //Not vulnerable
    if(ret == 0 && e1 == 0) return 1; //Vulnerable
    return errno == 0 || errno == 1 ? -1 : errno ; //Bad test
}

JNIEXPORT jint JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_kernel_CVE_12013_16282_checkGetPutUser(JNIEnv *env, jobject obj){
   return checkIsVulnerable();
}

int main(void){

  int r =  checkIsVulnerable();

  if(r == 0){
    printf("Device is vulnerable\n");
  }else if(r == 1){
    printf("Device is vulnerable\n");
  }else{
     printf("There was an error running tests\n");
  }

  return 0;
}
