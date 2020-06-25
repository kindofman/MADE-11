К сожалению плотно занялся конкурсом ближе к концу. Да и трудно было осилить такой объем информации.
Не успел реализовать все то, что хотел.

Прочел эти статьи
https://towardsdatascience.com/automatic-license-plate-detection-recognition-using-deep-learning-624def07eaaf
https://medium.com/data-science-in-your-pocket/vehicle-number-plate-detection-and-ocr-tcs-humain-2019-a253019e52a1
https://arxiv.org/abs/1709.08828

Что использовал:
1)Оптимайзер Adam
2)Шэдулер ReduceLROnplateu
3)Mask RCNN для детектирования
4)CTC loss(recognition)

Что заметил, но не успел обучить модель:
1)Модель использует и русские и английские буквы. При чем и большие и маленькие, что увеличивает количество выходов. Всегда имеет смысл разумно облегчать работу модели.
