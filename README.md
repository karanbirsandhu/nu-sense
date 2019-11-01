Nu-Sense
---

Nu-sense is a software that implements the data fusion algorithm  using principal component analysis by gathering data from multiple sensors and at first, a support degree matrix is established, Then we obtain each sensor’s integrated support degree score by PCA. According to their scores, we decide validity of sensor’s observation data. For abnormal observation data, they must be deleted before fusion. For the rest valid observation data, they are allocated corresponding weight coefficient in the light of their integrated support degree scores. Finally the fusion results are obtained. The proposed method objectively reflects the mutual support degree of sensors without knowing prior knowledge and assuming independent sensor noises.

By properly administrating multiple sensors and their measurements, it can achieve improved accuracy and more specific inferences than those using a single sensor alone

This implementation uses an algorithm discussed in this paper:

*"A Simple Multi-sensor Data Fusion Algorithm Based on Principal Component Analysis"* by GAO Hongyan

Project Partners
---
*Karanbir Singh Sandhu*    karanbirsandhu@cmail.carleton.ca

*Akaljot Singh Dhillon*    akaljotsinghdhillon@cmail.carleton.ca

*Ripudaman Singh*          ripudamansidhu@cmail.carleton.ca
