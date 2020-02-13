// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Projections/PromptFinalState.hh"
#include "Rivet/Tools/AliceCommon.hh"
#include "Rivet/Projections/AliceCommon.hh"
#include "Rivet/Tools/RivetYODA.hh"

namespace Rivet {


  /// @brief Add a short analysis description here
  class Jet_Raa_Analysis : public Analysis {
  public:

    /// Constructor
    DEFAULT_RIVET_ANALYSIS_CTOR(Jet_Raa_Analysis);


    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

        
        collSystem = getOption<string>("coll","PPMOD");
        set<string> done;  
      
        cout << "collSystem: " << collSystem << endl;
        
        if(collSystem == "PP")
        {
            book(jetPt,"pp_pt",10,0.,10.);
            book(sow,"pp_sow");
        }
        else if(collSystem == "PPMOD")
        {
            book(jetPt,"PbPb_pt",10,0.,10.);
            book(sow,"PbPb_sow");
            book(jetRaa, "jetRaa");
        }
        
        
        
        const FinalState fs(Cuts::pT > 150*MeV && Cuts::abseta < 0.9 && Cuts::abscharge > 0);
        declare(fs,"fs");
        
        fastjet::AreaType fjAreaType = fastjet::active_area_explicit_ghosts;
        //fastjet::GhostedAreaSpec(MaxRap, NGhostRepeats, GhostArea, GridScatter, KtScatter, MeanGhostKt);
        fastjet::GhostedAreaSpec fjGhostAreaSpec = fastjet::GhostedAreaSpec(1., 1, 0.005, 1., 0.1, 1e-100);
        fjAreaDef = new fastjet::AreaDefinition(fjGhostAreaSpec, fjAreaType);
        const FastJets jetsFJ(fs, fastjet::JetAlgorithm::antikt_algorithm, fastjet::RecombinationScheme::pt_scheme, jetR, fjAreaDef, JetAlg::Muons::NONE, JetAlg::Invisibles::NONE);
        declare(jetsFJ, "jets");

    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
        
        sow->fill();
        
        FastJets fj = applyProjection<FastJets>(event, "jets");
        Jets jets = fj.jetsByPt(Cuts::abseta < (0.9-jetR) && Cuts::pT > 0.15*GeV);
        
        for(Jet& jet : jets)
        {
            jetPt->fill(jet.pT()/GeV);
        }
      
      

    }


    /// Normalise histograms etc., after the run
    void finalize() {

        jetPt->scaleW(1./sow->sumW());
        
        if(collSystem == "PPMOD")
        {
            YODA::Histo1DPtr jetPt_pp = getPreload<Histo1D>("/Jet_Raa_Analysis:coll=PP/pp_pt");
            Histo1DPtr reference = registerAO<Rivet::Histo1D>(*jetPt_pp);
            YODA::CounterPtr sow_pp = getPreload<Counter>("/Jet_Raa_Analysis:coll=PP/pp_sow");
            CounterPtr ref_sow = registerAO<Rivet::Counter>(*sow_pp);
            reference->scaleW(1./ref_sow->sumW());
            divide(jetPt, reference, jetRaa);
        }
        

    }

    //@}


    /// @name Histograms
    //@{
    string collSystem;
    Histo1DPtr jetPt;
    Scatter2DPtr jetRaa;
    double jetR = 0.2;
    double jetAreaCut = 0.557*M_PI*jetR*jetR;
    fastjet::AreaDefinition *fjAreaDef;
    CounterPtr sow;
    //@}


  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(Jet_Raa_Analysis);


}
