//
//  main.cpp
//  testProject
//
//  Created by Stefan Kerschbaumer on 14.07.17.
//  Copyright © 2017 Kerschbaumer Stefan. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <iosfwd>
#include <fstream>
using namespace std;

#include "process.h"
#include "DylibHelper.h"
int exit_(string str) {
    printf("Exiting! %s", str.c_str());
    return 0;
}
int main(int argc, const char * argv[]) {

    attached_Process p = AttachToProcess("Skype");
    if(!p.setup(true))
        return exit_("Couldn't setup/ attach!");
    /*
    // Read Process memory Test
    int tmp  = 0;
    tmp = p.read<int>((mach_vm_address_t)0x114C2EE84);
    if(p.lastReadSucceeded())
        printf("\nInteger: %i", tmp);
    */
    
    DylibHelper *dylibH = new DylibHelper(p.getTask(), p.getName());
    if(!dylibH->getAllBaseAddresses())
        return exit_("Couldn't get all base addresses");
    
    DylibHelper::images_t *images = dylibH->returnImages();
    /*for(int i= 0; i < dylibH->allDylibCount; i++) {
        printf("DYLIB-Path: %s\nName: %s\nBase: 0x%llx \nSize: %lld\n", images[i].path, images[i].name, (mach_vm_address_t)images[i].baseAddress, images[i].st.st_size);
        
    }*/
    
    
    /*

    
    DylibHelper *dylibH = new DylibHelper(p.getTask());
    if(!dylibH->getAllBaseAddresses())
        return 0;
    
    DylibHelper::images_t *images = dylibH->returnImages();
    for(int i= 0; i < dylibH->allDylibCount; i++) {
        printf("DYLIB-Path: %s\nName: %s\nBase: 0x%llx \nSize: %lld\n", images[i].path, images[i].name, (mach_vm_address_t)images[i].baseAddress, images[i].st.st_size);
        
    }
     */
    return 0;
}
