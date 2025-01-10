

#include "server_instance.hpp"
#include "logger/logger.hpp"

namespace mdh {

server_instance::server_instance(
    std::shared_ptr<io_executor> io_executor,
    std::shared_ptr<asio::ssl::context> ssl_ctx,
    const market_data_config& config,
    uint32_t core_id
) noexcept : 
    io_exec_(std::move(io_executor)),
    ssl_ctx_(std::move(ssl_ctx)),
    config_(config),
    core_id_(core_id)
{
    assert(io_exec_ && "io_exec_ is null in server_instance constructor");
    assert(ssl_ctx_ && "ssl_ctx_ is null in server_instance constructor");

    conn_pool_ = std::make_unique<conn_pool_impl>(
        io_exec_,
        ssl_ctx_,
        core_id_,
        config_
    );    
}

auto server_instance::start() noexcept -> void {
    TRACE_LOG("server_instance::start() core_id: {}", core_id_);
    io_exec_->start();
}

auto server_instance::stop() noexcept -> void {
    TRACE_LOG("server_instance::stop() core_id: {}", core_id_);
}

server_instance::~server_instance() {
    TRACE_LOG("server_instance destructor core_id: {}", core_id_);
}
} // namespace mdh
