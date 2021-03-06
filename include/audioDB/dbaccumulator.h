template <class T> class DBAccumulator : public Accumulator {
public:
  DBAccumulator(unsigned int pointNN);
  ~DBAccumulator();
  void add_point(adb_result_t *r, double *thresh = NULL);
  double threshold(const char *);
  adb_query_results_t *get_points();
private:
  unsigned int pointNN;
  double _threshold;
  std::priority_queue< adb_result_t, std::vector<adb_result_t>, T > *queue;
};

template <class T> DBAccumulator<T>::DBAccumulator(unsigned int pointNN)
  : pointNN(pointNN), queue(0) {
  queue = new std::priority_queue< adb_result_t, std::vector<adb_result_t>, T>;
  _threshold = std::numeric_limits<double>::infinity();
}

template <class T> DBAccumulator<T>::~DBAccumulator() {
  if(queue) {
    delete queue;
  }
}

template <class T> void DBAccumulator<T>::add_point(adb_result_t *r, double *thresh) {
  if(!isnan(r->dist)) {
    queue->push(*r);
    if(queue->size() > pointNN) {
      queue->pop();
    }
    if(queue->size() == pointNN) {
      _threshold = queue->top().dist;
      if(thresh) {
        *thresh = _threshold;
      }
    }
  }
}

template <class T> double DBAccumulator<T>::threshold(const char *key) {
  return _threshold;
}

template <class T> adb_query_results_t *DBAccumulator<T>::get_points() {
  unsigned int size = queue->size();
  adb_query_results_t *r = (adb_query_results_t *) malloc(sizeof(adb_query_results_t));
  adb_result_t *rs = (adb_result_t *) calloc(size, sizeof(adb_result_t));
  r->nresults = size;
  r->results = rs;

  for(unsigned int k = 0; k < size; k++) {
    rs[k] = queue->top();
    queue->pop();
  }
  return r;
}
