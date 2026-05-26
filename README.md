### NoiseFilter
The class takes the grayscale difference image and cleans it into a binary mask suitable for contour detection. Since absolute differencing picks up minor lighting variation and sensor noise, four operations are applied in this order: 

      1. Thresholding cuts out weak pixel differences
      2. Gaussian blur smooths jagged edges
      3. Morphological opening removes small isolated speckle noise
      4. Morphological closing fills holes within genuine changed regions

### RegionHighlighter
This class takes the binary mask from NoiseFilter and identifies the specific regions where there are changes. It uses contour detection to find connected groups of white pixels, filters out anything too small to be a meaningful difference, and wraps the left over region in a bounding box drawn onto the original image. The output is an image that marks every change. It will also create region data (position, size, area) that can be used for further analysis or passed to evaluation for scoring with the ground truth of our testing dataset.

### Evaluation
This class measures how accurately the pipeline detects real changes by comparing the predicted binary mask against the ground truths provided by the LEVIR-CD dataset. It computes four metrics: 

      1. Precision (how many detections were correct)
      2. Recall (how many changes were found)
      3. F1 (the balance between the two above)
      4. Intersection over Union / IoU (the ratio of overlap to total area covered)
