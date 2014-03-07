#include "Router.h"

Router::Router()
{
}

Router::~Router()
{
}

//void Router::set_tcp_controller(ITCPController *tcp_controller)
//{
//    _tcp_controller = tcp_controller;
//}

void Router::set_models_controller(int id_tree, ITreeModel *models_controller)
{
    _map_model_controllers.insert(id_tree, models_controller);
}

//ITCPController *Router::get_tcp_controller()
//{
//    return _tcp_controller;
//}

ITreeModel *Router::get_models_controller(int id_tree)
{
    return _map_model_controllers.value(id_tree);
}
