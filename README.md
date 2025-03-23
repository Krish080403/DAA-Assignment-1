# Maximal Clique Finding Algorithms  

**[Link to the Webpage consisting of all Results](https://krish080403.github.io/DAA-Assignment-1/Hosting/Main.html)**  

## 👥 Names and Contributions:  
- **Krish Mantri (2021B3A71732H)**  
  - Implementation of Worst Case Algorithm  
  - Creation of Readme file  
  - Webpage Hosting  

- **Ananya Jalan (2021B3A72963H)**  
  - Implementation of Worst Case Algorithm  
  - Creation of Readme file  
  - Webpage Hosting  

- **Vinay Naidu Vanka (2021B3A71026H)**  
  - Implementation of Near Optimal Algorithm  
  - Webpage Hosting  

- **Sudarsan Mohanchander (2021B3A70558H)**  
  - Implementation of Optimal Algorithm  
  - Webpage Hosting  

- **Mandar Naphade (2021B3A72734H)**  
  - Implementation of Near Optimal Algorithm  
  - Webpage Hosting  

## 📖 Project Description  
This project focuses on finding **maximal cliques** in graphs by implementing algorithms from three well-known research papers.  

## ✅ What are Maximal Cliques?  
A **maximal clique** in a graph is a clique (a set of vertices where every two distinct vertices are adjacent) that cannot be extended by including any adjacent vertex; in other words, it's not a subset of a larger clique. Finding maximal cliques has applications in social network analysis, bioinformatics, and data mining.  

## 📚 Implemented Papers  
We have implemented algorithms from the following research papers:  

1. **Paper 1**:  
*The Worst-Case Time Complexity for Generating All Maximal Cliques and Computational Experiments*  
by Etsuji Tomita, Akira Tanaka, Haruhisa Takahashi  

2. **Paper 2**:  
*Listing All Maximal Cliques in Sparse Graphs in Near-optimal Time*  
by David Eppstein, Maarten Löffler, and Darren Strash  

3. **Paper 3**:  
*Arboricity and Subgraph Listing Algorithms*  
by Norishige Chiba and Takao Nishizeki  

## 📂 Code Files  
- [paper1.cpp](https://github.com/Krish080403/DAA-Assignment-1/blob/main/Code%20Files/paper1.cpp) – Implementation of Paper 1  
- [paper2.cpp](https://github.com/Krish080403/DAA-Assignment-1/blob/main/Code%20Files/paper2.cpp) – Implementation of Paper 2  
- [paper3.cpp](https://github.com/Krish080403/DAA-Assignment-1/blob/main/Code%20Files/paper3.cpp) – Implementation of Paper 3  

## ▶️ How to Run  
1. Download the code files and test cases from the [Test Case folder](https://github.com/Krish080403/DAA-Assignment-1/tree/main/Test%20Cases).  
2. Open a terminal and navigate to the directory where the files are saved.  
3. Run the code using the following commands:  
   - For Paper 1:  
     ```bash
     g++ paper1.cpp -o paper1
     ./paper1 <testcase_name.txt>
     ```  
   - For Paper 2:  
     ```bash
     g++ -std=c++11 paper2.cpp -o paper2
     ./paper2 <testcase_name.txt>
     ```  
   - For Paper 3:  
     ```bash
     g++ paper3.cpp -o paper3
     ./paper3 <testcase_name.txt>
     ```  
