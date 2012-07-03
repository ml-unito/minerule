class Safety : public OptionBase {
  bool overwriteHomonymMinerules;
  bool allowCascadeDeletes;
public:
  Safety() : overwriteHomonymMinerules(false), 
		 allowCascadeDeletes(false) {}
  virtual ~Safety() {}

  virtual std::string className() const {
	return "safety";
  }
  
  virtual void setOption(const std::string& name, const std::string& value)
	throw(MineruleException);

  bool getOverwriteHomonymMinerules() const { return overwriteHomonymMinerules; };
  void setOverwriteHomonymMinerules(bool rhs) { overwriteHomonymMinerules = rhs; };

  bool getAllowCascadeDeletes() const { return allowCascadeDeletes; }
  void setAllowCascadeDeletes(bool rhs) { allowCascadeDeletes = rhs; }
};
