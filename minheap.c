
#include <iostream>
#include <vector>

class MinHeap
{
    public:
        MinHeap()
            :nSize(0)
        {
        }
    public:
        int nSize;
        std::vector<int> m_vecHeap;

    public:
        void push(int nNum)
        {
            nSize++;
            int nIndex = nSize - 1;
            std::cout<< nIndex << std::endl;
            m_vecHeap.resize(nIndex + 1);
            if(nIndex >= 0) m_vecHeap[nIndex] = nNum;
            if(nIndex < 0) return;
            int nTemp = m_vecHeap[nIndex];
            while(nIndex > 0 && m_vecHeap[(nIndex -1)/2] > nTemp)
            {
                m_vecHeap[nIndex] = m_vecHeap[(nIndex - 1)/2];
                m_vecHeap[(nIndex -1)/2] = nTemp;
                nIndex = (nIndex -1)/2;
            }
            
        }

        void pop()
        {
            
            m_vecHeap[0] = m_vecHeap[nSize - 1];
            m_vecHeap.pop_back();
            nSize--;

            if(nSize <= 0) return;
            int nIndex = 0;
            int nTemp = m_vecHeap[nIndex];
            while(nSize > nIndex)
            {
                std::cout << nIndex << std::endl;
                if(nIndex + 1 < nSize && m_vecHeap[nIndex+1] < m_vecHeap[nIndex])
                {
                    
                    m_vecHeap[nIndex] = m_vecHeap[nIndex+1];
                    m_vecHeap[nIndex+1] = nTemp;
                    nTemp = m_vecHeap[nIndex];
                    nIndex = nIndex + 1;
                }
                
                if (nIndex + 1 < nSize && m_vecHeap[nIndex + 1] < nTemp)
                {
                    m_vecHeap[nIndex - 1] = m_vecHeap[nIndex+1];
                    m_vecHeap[nIndex+1] = nTemp;
                    nTemp = m_vecHeap[nIndex + 1];
                    nIndex = nIndex + 1;
                }
                else{
                    break;
                }
                print();
            }
        }

        void print()
        {
            for(size_t i = 0 ; i < m_vecHeap.size(); ++i)
            {
                std::cout << m_vecHeap[i] << " ";
            }
            std::cout << std::endl;
        }
};



int main()
{
    MinHeap heap;
    heap.push(11);
 heap.push(1);
 heap.push(98);
 heap.push(3);
 heap.push(7);
 heap.push(2);
 heap.push(77);
 heap.push(8);

 heap.print();
 heap.pop();
 heap.print();
    return 0;
}
