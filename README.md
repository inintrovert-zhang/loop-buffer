# loop-buffer
描述： 	此文档提供了对数据帧环形缓冲区的队列操作。适用于数据帧长度不固定，每次入列的
     数据，在出列操作能完整读出来，一般用于通信数据缓存。

注意：数据帧的长度信息和数据信息都存放在同一个缓冲区，数据帧长度信息占用2个字节

**谨慎使用，可做参考**
