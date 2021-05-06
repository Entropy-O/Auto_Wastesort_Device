# coding=utf-8
import difflib


def Evaluator(result):
    accuracy = 0.6
    kehuishou = ['yilaguan', 'suliaoping',]  # 可回收

    youhai = ['dianchi,niukou']  # 有害

    chuyu = ['pingguo', 'xiangjiao', 'juzi','qingjiao','tudo','xihongshi','huluobo','shuiluobo'] 
    qita = ['yantou','suicipian','zhuankuai']
    A = difflib.get_close_matches(result, kehuishou, 2, cutoff=accuracy)
    B = difflib.get_close_matches(result, youhai, 1, cutoff=accuracy)
    C = difflib.get_close_matches(result, chuyu, 2, cutoff=accuracy)
    D = difflib.get_close_matches(result, qita, 2, cutoff=accuracy)
    if len(A) != 0 and result in set(A):  # 返回的是列表，需要进行列表比对！2019.10.17
        return 3
    elif result == 'niukou' or result =='dianchi':
        return 1
    elif len(C) != 0 and result in set(C):
        return 0
    elif len(D) != 0 and result in set(D):
        return 2
    else:
        return -1


def ChineseConvert(pin_res):
    if pin_res == 'yilaguan':
        return '易拉罐'
    elif pin_res == 'suliaoping':
        return '塑料瓶'
    elif pin_res == 'dianchi':
        return '电池'
    elif pin_res == 'xiangjiao':
        return '香蕉'
    elif pin_res == 'pingguo':
        return '苹果'
    elif pin_res == 'juzi':
        return '橘子'
    elif pin_res == 'qingjiao':
        return '青椒'
    elif pin_res == 'yantou':
        return '烟头'
    elif pin_res == 'xihongshi':
        return '西红柿'
    elif pin_res == 'niukou':
        return '电池'
    elif pin_res == 'zhuankuai':
        return '砖块'
    elif pin_res == 'suicipian':
        return '碎瓷片'
    elif pin_res == 'tudo':
        return '土豆'
    elif pin_res == 'huluobo':
        return '萝卜'
    elif pin_res == 'shuiluobo':
        return '萝卜'
    

