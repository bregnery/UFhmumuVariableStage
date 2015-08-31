#!/usr/bin/env python

import glob
import ROOT as root
from src.helpers import *

root.gROOT.SetBatch(True)

vhmumu = root.TFile("VHmumu_13TeV_Hist.root")

canvas = root.TCanvas()

phiStar = vhmumu.Get("phiStarHist")
phiStarCheck = vhmumu.Get("phiStarHist")

xMin = 0
xMax = 1
yMin = 0
yMax = 3000
xTitle = "#phi *"

axisHist = root.TH2F("axisHist","",1,xMin,xMax,1,yMin,yMax)
axisHist.GetXaxis().SetTitle(xTitle)
axisHist.GetYaxis().SetTitle("Events/Bin")
axisHist.Draw()

phiStar.SetLineColor(1)
phiStar.SetMarkerStyle(0)
phiStar.SetLineStyle(1)
phiStarCheck.SetLineColor(2)
phiStarCheck.SetMarkerStyle(0)
phiStarCheck.SetLineStyle(1)

leg = root.TLegend(.2,.7,.4,.9,"MC sample")
leg.AddEntry(phiStar,"#phi *","l")
leg.AddEntry(phiStarCheck,"#phi * Check","l")

phiStar.Draw("HIST same")
phiStarCheck.Draw("HIST same")
leg.Draw()

canvas.SaveAs("Hist_phiStarCheck.png")


