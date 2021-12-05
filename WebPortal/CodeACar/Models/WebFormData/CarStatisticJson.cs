using System;
using System.Collections.Generic;

namespace CodeACar.Models.WebFormData
{
    public class CarStatisticJson
    {
        public int ChallengeHistoryId { get; set; }
        public List<CarStatisticObject> Statistic { get; set; }
    }
}
