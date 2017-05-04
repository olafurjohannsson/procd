
class memory_mappable
{
  private:
    // Non-copyable
    memory_mappable(const memory_mappable &);

    // Non-assignable
    memory_mappable &operator=(const memory_mappable &);

  public:
    typedef /*implementation-defined*/ accessmode_t;
    typedef /*implementation-defined*/ mapping_handle_t;
    static const mapping_handle_t invalid_handle;
    static const accessmode_t read_only;
    static const accessmode_t read_write;
    static const accessmode_t invalid_mode;

  public:
    memory_mappable();

  protected:
    void init(mapping_handle_t handle, accessmode_t mode);

  public:
    memory_mappable(memory_mappable &&other);

    mapping_handle_t get_handle() const;

    accessmode_t get_mode() const;

    virtual ~memory_mappable() = 0;
};