# NaaVRE aneris-dna flavor

## Build & run

### Build

```shell
docker build . -f flavors/aneris-dna/cell-runtime.Dockerfile --build-arg CONDA_ENV_FILE=flavors/aneris-dna/environment.yaml -t naavre-fl-aneris-dna-runtime:local

docker build . -f docker/cell-build.Dockerfile               --build-arg CONDA_ENV_FILE=flavors/aneris-dna/environment.yaml -t naavre-fl-aneris-dna-build:local

docker build . -f flavors/aneris-dna/jupyter.Dockerfile      --build-arg CONDA_ENV_FILE=flavors/aneris-dna/environment.yaml -t naavre-fl-aneris-dna-jupyter:local
```

### Run

```shell
docker run -it -p 8888:8888 --name aneris-dna-jupyter --volume="//c/DockerShare/ANERIS_DNA:/home/jovyan" naavre-fl-aneris-dna-jupyter:local
```

## PEMA

### env

```shell
docker run -it --name pema --volume="//c/DockerShare/ANERIS_DNA/PEMA/analysis:/mnt/analysis" hariszaf/pema:v.2.1.4
```

### workspace

```shell
root@8397d391df3c:/home# ls /home/
GUniFrac  R-3.6.0  cmake-3.21.4  modules  pema_R_packages.tsv  pema_environment.tsv  pema_latest.bds  scripts  tools

root@8397d391df3c:/home# ls /mnt/analysis/
mydata  parameters.tsv  pema_latest.bds
```

### exec

* parameters.tsv: 
  * `outputFolderName	test_18S`
  * `gene	gene_16S`
* log: `pema_latest.log`
* tmp: `*.chp`
* result: 
  * `7.mainOutput\gene_16S\vsearch\my_taxon_assign\finalTable.tsv`
  * `7.mainOutput\gene_16S\vsearch\all_sequences_grouped.fa`
  * parameters.tsv: `parameters0f.test_18S.tsv`
* pema_analysis_dir.zip: `test_18S\*`

#### test_18S, gene_16S

```shell
root@8397d391df3c:/home# ./pema_latest.bds 2>&1 | tee /mnt/analysis/pema_latest.log

Picked up JAVA_TOOL_OPTIONS: -XX:+UseContainerSupport
A new output files was just created!
ERR7125480_1.fastq.gz
ERR7125480_2.fastq.gz
ERR7125483_1.fastq.gz
ERR7125483_2.fastq.gz
ERR7125486_1.fastq.gz
ERR7125486_2.fastq.gz
ERR7125489_1.fastq.gz
ERR7125489_2.fastq.gz
Picked up JAVA_TOOL_OPTIONS: -XX:+UseContainerSupport
...
  inflating: ERR7125489_1_fastqc/fastqc.fo
here is readDirection from sample
ERR7125489_2_fastqc.zip
2
Archive:  /mnt/analysis/test_18S/1.qualityControl/ERR7125489_2_fastqc.zip
   creating: ERR7125489_2_fastqc/
   creating: ERR7125489_2_fastqc/Icons/
   creating: ERR7125489_2_fastqc/Images/
  inflating: ERR7125489_2_fastqc/Icons/fastqc_icon.png
  inflating: ERR7125489_2_fastqc/Icons/warning.png
  inflating: ERR7125489_2_fastqc/Icons/error.png
  inflating: ERR7125489_2_fastqc/Icons/tick.png
  inflating: ERR7125489_2_fastqc/summary.txt
  inflating: ERR7125489_2_fastqc/Images/per_base_quality.png
  inflating: ERR7125489_2_fastqc/Images/per_tile_quality.png
  inflating: ERR7125489_2_fastqc/Images/per_sequence_quality.png
  inflating: ERR7125489_2_fastqc/Images/per_base_sequence_content.png
  inflating: ERR7125489_2_fastqc/Images/per_sequence_gc_content.png
  inflating: ERR7125489_2_fastqc/Images/per_base_n_content.png
  inflating: ERR7125489_2_fastqc/Images/sequence_length_distribution.png
  inflating: ERR7125489_2_fastqc/Images/duplication_levels.png
  inflating: ERR7125489_2_fastqc/Images/adapter_content.png
  inflating: ERR7125489_2_fastqc/fastqc_report.html
  inflating: ERR7125489_2_fastqc/fastqc_data.txt
  inflating: ERR7125489_2_fastqc/fastqc.fo
----------------------------------------------------------
Pema has been completed successfully. Let biology start!
Thanks for using Pema.
```

#### Res_gene_18S-PEMA_v2.1.4-docker, gene_18S

* parameters.tsv: 
  * `outputFolderName	Res_gene_18S-PEMA_v2.1.4-docker`
  * `gene	gene_18S`
* log: `Res_gene_18S-PEMA_v2.1.4-docker.log`
* tmp: `*.chp`
* result: 
  * `7.mainOutput\gene_18S\vsearch\my_taxon_assign\finalTable.tsv`
  * `7.mainOutput\gene_18S\vsearch\all_sequences_grouped.fa`
  * parameters.tsv: `parameters0f.Res_gene_18S-PEMA_v2.1.4-docker.tsv`
* pema_analysis_dir.zip: `Res_gene_18S-PEMA_v2.1.4-docker\*`

```shell
root@8397d391df3c:/home# ./pema_latest.bds 2>&1 | tee /mnt/analysis/Res_gene_18S-PEMA_v2.1.4-docker.log

root@8397d391df3c:/home# cp -rf /mnt/analysis/mydata                              /mnt/analysis/Res_gene_18S-PEMA_v2.1.4-docker/
root@8397d391df3c:/home# mv     /mnt/analysis/Res_gene_18S-PEMA_v2.1.4-docker.log /mnt/analysis/Res_gene_18S-PEMA_v2.1.4-docker/

root@8397d391df3c:/home# ls
GUniFrac      modules               pema_latest.bds                      pema_latest.bds.20251205_133048_926  pema_latest.log
R-3.6.0       pema_R_packages.tsv   pema_latest.bds.20251205_132857_377  pema_latest.bds.20251205_141345_284  scripts
cmake-3.21.4  pema_environment.tsv  pema_latest.bds.20251205_133032_618  pema_latest.bds.20251205_141456_171  tools
```

#### compare, gene_16S & gene_18S

`finalTable.tsv`: match

### AliView, FastTree, FigTree

#### C:\MyPrograms\FastTree\FastTree.exe" -nt -gtr -out TEMP_OUT_FILE CURRENT_ALIGNMENT_FASTA

```shell
C:\MyPrograms\FastTree\FastTree.exe -nt -gtr -out C:\Users\quan.pan\AppData\Local\Temp\aliview-tmp-tempfile-for-new-alignment_6002128974095716153.tmp C:\Users\quan.pan\AppData\Local\Temp\aliview-tmp-current-alignment_7281490812908973559fas

FastTree Version 2.2.0 Double precision
Alignment: C:\Users\quan.pan\AppData\Local\Temp\aliview-tmp-current-alignment_7281490812908973559fas
Nucleotide distances: Jukes-Cantor Joins: balanced Support: SH-like 1000
Search: Normal +NNI +SPR (2 rounds range 10) +ML-NNI opt-each=1
TopHits: 1.00*sqrtN close=default refresh=0.80
ML Model: Generalized Time-Reversible, CAT approximation with 20 rate categories
Non-unique name 'ERR7125483' in the alignment
```

#### C:\MyPrograms\FigTree\FigTree v1.4.4.exe" TEMP_OUT_FILE

require java
* Version 8 Update 471
* Release date: October 21, 2025
* filesize: 38.48 MB

```shell
C:\MyPrograms\FigTree\FigTree v1.4.4.exe C:\Users\quan.pan\AppData\Local\Temp\aliview-tmp-tempfile-for-new-alignment_6002128974095716153.tmp

javax.swing.UIManager$LookAndFeelInfo[Metal javax.swing.plaf.metal.MetalLookAndFeel]
javax.swing.UIManager$LookAndFeelInfo[Nimbus javax.swing.plaf.nimbus.NimbusLookAndFeel]
javax.swing.UIManager$LookAndFeelInfo[CDE/Motif com.sun.java.swing.plaf.motif.MotifLookAndFeel]
javax.swing.UIManager$LookAndFeelInfo[Windows com.sun.java.swing.plaf.windows.WindowsLookAndFeel]
javax.swing.UIManager$LookAndFeelInfo[Windows Classic com.sun.java.swing.plaf.windows.WindowsClassicLookAndFeel]
```