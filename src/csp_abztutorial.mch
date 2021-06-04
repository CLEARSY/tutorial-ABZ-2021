/*
This work is licensed under the Creative Commons Attribution 4.0 International License. 
To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/
*/

MACHINE
    csp_abztutorial
    
SEES
    csp_stdLib_type,
    csp_cs0Lib

ABSTRACT_VARIABLES
    v_lc_clock
    
INVARIANT
    v_lc_clock : uint64_t
    
INITIALISATION
    v_lc_clock := 0
    
OPERATIONS
    
    r_f__lc_treatement(p_clockH, p_clockL, p_inLc, p_outLc) =
    PRE
        p_clockH : uint32_t &
        p_clockL : uint32_t &
        p_inLc : uint8_t &
        p_outLc : uint8_t &
        v_watchdogLiveness_clock = v_lc_clock
    THEN
        skip
    END;
    
    lc_watchdogTimer =
    PRE
        v_watchdogLiveness_clock - 1 = v_lc_clock 
    THEN
        v_lc_clock := v_lc_clock + 1
    END;
    
    po <-- r_f__lc_get_freeZone =
    PRE
        po : uint8_t
    THEN
        po :: uint8_t
    END;
    
    p_address, p_value <-- r_f__lc_getSetAddressAndValue =
    PRE
        p_address : uint32_t &
        p_value : uint32_t 
    THEN
        p_address,p_value :(p_address : uint32_t 
            & p_value : uint32_t)
    END;
    
    p_address, p_value <-- r_f__lc_getClearAddressAndValue =
    PRE
        p_address : uint32_t &
        p_value : uint32_t 
    THEN
        p_address,p_value :(p_address : uint32_t 
            & p_value : uint32_t)
    END
    
END
