import cv2


def main():

    # 1.初始化读取视频对象
    cap = cv2.VideoCapture("/home/pi/Downloads/1.mp4")

    # 2.循环读取图片
    while cap.isOpened():
        ret, frame = cap.read()
        if ret:
            cv2.imshow("frame", frame)
        else:
            print("视频播放完成！")
            break

        # 退出播放
        key = cv2.waitKey(25)
        if key == 27:  # 按键esc
            break

    # 3.释放资源
    cap.release()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    main()

