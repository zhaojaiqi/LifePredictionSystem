#include "PyWorker.h"

#include "PyCore.h"

#include <Python.h>
#include <thread>

Status PyWorker::RunPyScriptSync(const PyTask &task, std::function<void(string)> invokeAfterComplete) {
    Status result = Status::FAILURE;

    PyGILState_STATE gstate = PyGILState_Ensure();

    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    pModule = PyImport_ImportModule(task.module.c_str());

    if (pModule)
    {
        pFunc = PyObject_GetAttrString(pModule, task.entryFunction.c_str());

        if (pFunc && PyCallable_Check(pFunc))
        {
            int argc = task.args.size();

            pArgs = argc ? PyTuple_New(argc) : nullptr;

            for (int i = 0; i < argc; ++i)
                PyTuple_SetItem(pArgs, i, PyUnicode_DecodeFSDefault(task.args[i].c_str()));

            pValue = PyObject_CallObject(pFunc, pArgs);

            const char* response;
            Py_ssize_t size;

            if (pValue && (response = PyUnicode_AsUTF8AndSize(pValue, &size)))
            {
                result = Status::SUCCESS;
                invokeAfterComplete(string(response, size));
            }
            else
                LOG("PyWorker: Call Function Failed!");
        }
        else
            LOG("PyWorker: Can't Find Function");
    }
    else
        LOG("PyWorker: Can't Find Module(%s)", task.module.c_str());

    PyGILState_Release(gstate);

    return result;
}

Status PyWorker::RunPyScriptAsync(const PyTask &task, std::function<void(string)> invokeAfterComplete) {
    std::thread thread([=]{
        PyWorker::RunPyScriptSync(task, invokeAfterComplete);
    });

    thread.detach();
}
