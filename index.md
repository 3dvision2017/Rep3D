# Introduction
Applications like scene modeling, autonomous navigation and virtual tourism make use of sparse point clouds obtained via Structure-from-Motion. Often these applications require new images to be localized within the preceding point cloud. In the presence of repetitive structures (e.g. windows, cladding, panels) this task becomes very difficult, since the localization might be ambiguous. Augmenting the point cloud with information on repetitive structures has two potential benefits. First the information could be used to rectify and pack the sparse point cloud. Second knowing more about the appearance of repetitions in the point cloud might help subsequent localization steps to improve their performance.

# Problem Definition and Technical Details

There are four main problems to be solved of which the first two are already solved by previous work: 

1. Obtain a 3D reconstruction (we use COLMAP2)
2. Find repetitions on 2D images (we use REP4)
3. Find 3D projection planes  (our contribution)
4. Project repetitions from 2D to 3D (our contribution)
5. Find optimal repetition representation in point cloud (our contribution)

To find the projection planes for point 3 and 4 we use corresponding features that lie within the repetitions in 2D and use RANSAC to fit the plane model. We then project every 2D repetition onto its corresponding plane in 3D, found before. This results in multiple overlapping repetitions in 3D. Then we use a Hough transform in combination with the DBSCAN algorithm to find the main planes (these correspond to the facades of the building). We then project the repetitions onto their corresponding facades. As a final step we cluster the repetitions on their main plane and find one optimal representation for each cluster. This is done by comparing overlap, cell width and height, corner closeness and the amount of columns and rows of a repetition scheme.

As a study case we use an image dataset of the CAB building at ETH Zurich.

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and 

```

![Pipeline](Rep3D/pipeline1.svg)

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/lfrschkn/Rep3D/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.
