#pragma once

#include "IPresenter.h"
#include "IView.h"
#include "CrapsBonusTracker.h"

#include <memory>

class CbtPresenter : public IPresenter
{
public:

    CbtPresenter(std::shared_ptr<CrapsBonusTracker> model, IView& view);
    ~CbtPresenter() override = default;

    void Run() override;

private:

    std::shared_ptr<CrapsBonusTracker> m_Model;    
    IView&  m_View;
    bool    m_Running;
    int     m_Roll;

};
