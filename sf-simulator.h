#ifndef SHARDFREE_SF_SIMULATOR_H_
#define SHARDFREE_SF_SIMULATOR_H_

class SFLogger;

class SFSimulator {
  public:
    static SFSimulator *create();
    void run();
    
  private:
    SFSimulator();
    virtual ~SFSimulator();
    
    void collect();
    void simulate();
    void resolveAndPush();
    
  private:
    int counter;
    SFLogger *loggerp;
};

#endif // SHARDFREE_SF_SIMULATOR_H_