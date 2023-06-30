from PPOCR_api import GetOcrApi
import os

def getResult(res: dict, result):
    # 识别成功
    if res["code"] == 100:
        index = 1
        for line in res["data"]:
            result += f"{index}-置信度：{round(line['score'], 2)}，文本：{line['text']}\n"
            index += 1
    elif res["code"] == 100:
        print("图片中未识别出文字。")
    else:
        print(f"图片识别失败。错误码：{res['code']}，错误信息：{res['data']}")

def RecognizeImage(imageBytes):
    # 初始化识别器对象，传入 PaddleOCR_json.exe 的路径
    # current_directory = os.getcwd()
    # srt = '\PaddleOCR-json_v.1.3.0\PaddleOCR-json.exe'
    # path = f'{current_directory}{srt}'
    # ocr = GetOcrApi(path)

    ocr = GetOcrApi(r"C:\Qt\code\Image_text_extraction\pyorc_Project\PaddleOCR-json_v.1.3.0\PaddleOCR-json.exe")
    # 调用OCR引擎进行文字识别
    res = ocr.runBytes(imageBytes)

    result = []  # 用于保存结果的列表
    getResult(res, result)
    final_result = "".join(result)  # 将列表转换为字符串

    return final_result
   

def RecognizeClipboard():
    # current_directory = os.getcwd()
    # srt = '\PaddleOCR-json_v.1.3.0\PaddleOCR-json.exe'
    # path = f'{current_directory}{srt}'
    # ocr = GetOcrApi(path)
    ocr = GetOcrApi(r"C:\Qt\code\Image_text_extraction\pyorc_Project\PaddleOCR-json_v.1.3.0\PaddleOCR-json.exe")
    res = ocr.runClipboard()
    if res["code"] == 212:
        final_result = "当前剪贴板中没有图片"
    else:
        result = []  # 用于保存结果的列表
        getResult(res, result)
        final_result = "".join(result)  # 将列表转换为字符串
    return final_result

if __name__ == '__main__':
    print("test")
    # with open("demo.jpg", "rb") as file:
    #     imageBytes = file.read()  # 将图像文件读取为字节流
    # print(RecognizeImage(imageBytes))
    # print(RecognizeClipboard())
