#include "system.h"
int main(){
    class system objSystemClass(nullptr);
    objSystemClass.build_tree();
    return objSystemClass.start_app();
}