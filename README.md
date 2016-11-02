### 2016 sogang-pintos project - made by Jeong Min Gyo

## Program flow
(skip initializing)

1. Start main from thread/init.c
  - Get argv and init.
  - call run\_actions
1. In run\_actions, call some function by function pointer.
  - In case of run, call run\_task
    - Case : run (run\_task)
    - process wait for task
      - call : process\_wait(process\_execute(task))
      - It runs at main thread.

## Synchronization strategy
Using semaphore

1. Parent thread make child thread(userprog/process.c - process\_execute
   thread\_create)
1. Then sema down parent thread to check if child thread loaded well.
1. process control passed to child thread. Child thread execute start\_process
   and load.
  - If load finished, check if load has succeeded.
  - Then do some jobs for each case.
    - If fail, remove thread from parent's child list.
    - If succeed, do normaly. 
  - After do jobs, pass control to parent by sema down the child and sema up
  the parent.
1. Process control passed to parent. (Thread running on process\_execute)
  - Check if loaded well. If not, return TID\_ERROR(-1). Else, return tid
1. I assumed that pintos must call wait after make thread.
  - In case of main, it must call wait after process\_execute.
  - In case of main's child, it will call wait as system call.
  - In wait call, it passed child's tid. So find child's thread by tid.
    - If child not find, return -1
    - If child already waited by parent, return -1
  - sema up child thread and sema down parent (So pass control to child)
    - Purpose : Wait child's job before parent dies.
1. Process control passed to child. (job : system call)
  - At this, pintos maybe make child's child(call it grandchild).
  - If grandchild maded, do upper jobs(1~5) in terms of child - grandchild.
  - Child's job mainly focused on system call(like write, exec, ...)
  - I assumed that child should call exit as system call.
    - At exit call, it check if there is unfinished job about grandchild 
      (check it recursively by job processing above)
    - After wait grandchild's thread, then print exit status and thread name
  - Child sema up parent's thread
    - Purpose : Parent thread want to get child's result status.
1. Process control passed to parent (job : just get child's result status)
  - After get status, just sema up child thread.
  - In this process, parent didn't sema down itself.
1. Process are running on one of both. (scheduled by running queue)
  - In case of child
    - it removes ifself's element of parent's child list.
    - Then call thread\_exit()
  - In case of parent
    - return child's status and do something.
    - In main, pintos finished.
    - Or any other main's child(or deeper), do their jobs like system call.

