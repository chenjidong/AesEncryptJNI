## 普通 jni aes 128 加解密
## 开发环境
### jdk 1.8.0_161
### android studio 3.4.1
### ndk 19.2.5345600

### 导出jar包 命令：gradle makeJar

### thanks 
#### logic：https://github.com/BruceWind/AESJniEncrypt
#### md5://github.com/wtuadn/JNITool
#### aes：https://github.com/kokke/tiny-AES-c
#### base64：https://github.com/willemt/pearldb


### AES基于tiny-AES128-C扩展，根据key长度自动选择AES128、AES192、AES256 默认采用PKCS7Padding填充（和PKCS5Padding一样），加密后进行一次Base64