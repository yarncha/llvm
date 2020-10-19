# LLVM Passes

📜 Lists of Passes
* IRPrinter
* ControlFlowEditor
* VirtualEditor

전체 코드가 아닌 기능을 작성한 부분만 올렸으며, 이후에 CMakeLists.txt와 전체 코드를 동작할 수 있게 올릴 예정

### IRPrinter

: LLVM IR을 프린트하여 확인할 수 있게 하는 Pass

### ControlFlowEditor
: Control Flow 난독화

### VirtualEditor

: **가상화 난독화**를 위한 Pass

* 내용물

| VirtualEditor.cpp   | virtualTest.c     | virtualTestObfuscated.c     |
| :------------- | :------------- | :------------- |
| 가상화 난독화를 위한 pass를 작성한 cpp파일 | 난독화하려는 c코드 | 난독화 이후 c 코드(virtualTest.c파일을 다음 파일처럼 변환하는 방식으로 pass 작성) |

* 설명

virtualTest.c파일을 virtualTestObfuscated.c와 같은 구조로 가상화 난독화를 적용해주는 패스. (해당 코드에서만 적용되며 이후 다른 c코드에 대해서도 적용할 수 있도록 추후 수정 예정)
