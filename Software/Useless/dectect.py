def dectect_loop():
    while True:
            global window
            #t.stop(3)
            #t.pause()
            time.sleep(3)
            res,time_used=SDK_Identify()
            
        #     if last_rec == res:
        #         i+=1
        #     else:
        #         i=0
        #     if i==2:
        #         print(res)
        #         i=0
        #     last_rec = res
            if res!='' and last_res!=res:
                num+=1
                window.state('normal')
                P1chufa(num,Evaluator(res),ChineseConvert(res),time_used)
                window.update()
                time.sleep(6)
                window.state('icon')
                #Pour_to(Evaluator(res))
                #window.destroy()
                last_res=res
        
