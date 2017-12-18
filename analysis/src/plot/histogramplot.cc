// Copyright 2017 Genti Saliu

#include <plot/histogramplot.h>
#include <TFile.h>
#include <TCanvas.h>

HistogramPlot::HistogramPlot() {
    this->x1 = -1;
    this->y1 = -1;
    this->x2 = -1;
    this->y2 = -1;
}

void HistogramPlot::addHist(TH1D* h, const EColor &color) {
    h->SetLineColor(color);
    h->Draw();
    gPad->Update();

    TPaveStats *stats = reinterpret_cast<TPaveStats*>(h->FindObject("stats"));
    this->positionStatsAfterPrevious(stats, color);

    this->histograms.push_back(h);
    this->stats.push_back(stats);
}

void HistogramPlot::positionStatsAfterPrevious(TPaveStats *stats,
                                               const EColor &color) {
    if (this->x1 != -1) {
        stats->SetTextColor(color);
        stats->SetLineColor(color);

        stats->SetX1NDC(this->x1);
        stats->SetX2NDC(this->x2);
        stats->SetY1NDC(this->y1 - (this->y2 - this->y1));
        stats->SetY2NDC(this->y1);
    }

    this->x1 = stats->GetX1NDC();
    this->y1 = stats->GetY1NDC();
    this->x2 = stats->GetX2NDC();
    this->y2 = stats->GetY2NDC();
}

void HistogramPlot::plot(const string &targetFile) {
    TCanvas c("canvas");

    for (unsigned int i = 0; i != this->histograms.size(); i++) {
        TH1D *h = this->histograms.at(i);

        if (i == 0)
            h->Draw();
        else
            h->Draw("same");

        this->stats.at(i)->Draw("same");
    }

    c.Print(targetFile.c_str());
}